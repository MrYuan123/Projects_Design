#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#include "protocol.h"
#include "datalink.h"

enum { data_frame = 1, ack_frame, nak_frame };     //用于判断传帧类型
#define DATA_TIMER   3400
#define ACK_TIMER    900

#define MAX_SEQ 31 
#define NR_BUFS 16    
#define inc(k) if(k < MAX_SEQ)k++;else k=0   //用于记录发送窗口上界

typedef struct FRAME {
	unsigned char kind;    
	unsigned char ack;
	unsigned char seq;
	unsigned char data[PKT_LEN];
	unsigned int padding;
} *Ptr_Frame, FRAME;
/*
next_frame_to_send 记录下一个需要发送的包的序号
frame_expected 记录下一个接收的包的序号
ack_expected 记录需要接受的ACK的序号
out_buffer[NR_BUFS][PKT_LEN] 缓存目前发送的包
in_buffer[NR_BUFS][PKT_LEN]  缓存接受的包
nbuffered 记录目前输出缓存中的个数
phl_ready 记录物理层是否可以发送数据
no_nak  用于记录是否已经发送NAK，若已发送，则不再重复发送
arrived[NR_BUFS] 用于标记该帧是否已经收到
too_far 标记目前接收的最大上界
*/
static unsigned char next_frame_to_send = 0, frame_expected = 0, ack_expected = 0;
static unsigned char out_buffer[NR_BUFS][PKT_LEN], in_buffer[NR_BUFS][PKT_LEN], nbuffered;
static unsigned char too_far = NR_BUFS;
static int phl_ready = 0;
bool arrived[NR_BUFS];
bool no_nak = true;

static bool between(int a, int b, int c) {
	return((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c <a));
}

static void put_frame(unsigned char *frame, int len) {
	*(unsigned int *)(frame + len) = crc32(frame, len);
	send_frame(frame, len + 4);
	phl_ready = 0;
}
void send_frame_ph(unsigned char frame_nr, int frame_kind) {
	FRAME s;
	switch (frame_kind) {
	case data_frame: {
		//printf("data_frame link\n");
		s.kind = FRAME_DATA;
		s.seq = frame_nr;
		s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
		memcpy(s.data, out_buffer[frame_nr % NR_BUFS], PKT_LEN);

		dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);
		put_frame((unsigned char *)&s, 3 + PKT_LEN);
		start_timer(frame_nr % NR_BUFS, DATA_TIMER);
		stop_ack_timer();
		break;
	}
	case ack_frame: {
		//printf("ack_frame link!\n");
		s.kind = FRAME_ACK;
		s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

		dbg_frame("Send ACK  %d\n", s.ack);

		put_frame((unsigned char *)&s, 2);
		stop_ack_timer();
		break;
	}
	case nak_frame: {
		s.kind = FRAME_NAK;
		s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

		no_nak = false;

		dbg_frame("Send NAK with ACK %d\n", s.ack);

		put_frame((unsigned char *)&s, 2);
		stop_ack_timer();
		break;
	}
	default: {
		lprintf("Error!\n");
		return;
		break;
	}
	}

	return;
}

int main(int argc, char **argv) {

	nbuffered = 0;
	int i;
	for (i = 0; i < NR_BUFS; i++)
		arrived[i] = false;

	int event, oldest_frame;
	FRAME f;
	int len = 0;

	protocol_init(argc, argv);
	lprintf("Designed by Yuan Yukun, based on Teacher Jiang's code.\nBuild: "__DATE__"  "__TIME__"\n");
	disable_network_layer();

	for (;;) {
		event = wait_for_event(&oldest_frame);

		switch (event) {
		case NETWORK_LAYER_READY: {
			get_packet(out_buffer[next_frame_to_send % NR_BUFS]);
			nbuffered++;
			send_frame_ph(next_frame_to_send, data_frame);
			inc(next_frame_to_send);
			break;
		}
		/*
		当为物理层等待时间时，说明可向物理层发送帧，因而首先先从网络层获取数据，存储在缓存中，并将输出缓存的
		容量增大，并利用send_frame_ph()函数向物理层发送数据，并将存储下帧发送数据的数值增加 1；
		*/

		case PHYSICAL_LAYER_READY: {
			phl_ready = 1;
			break;
		}
		/*
		当为物理层等待时间时，将phl_ready置为1,表示当前物理层传输可用；
		*/
		case FRAME_RECEIVED: {
			len = recv_frame((unsigned char *)&f, sizeof f);
			if (len < 5 || crc32((unsigned char *)&f, len) != 0) {
				dbg_event("**** Receiver Error, Bad CRC Checksum\n");
				if (no_nak)
					send_frame_ph(NULL, nak_frame);
				break;
			}
			if (f.kind == FRAME_ACK)
				dbg_frame("Recv ACK  %d\n", f.ack);

			if (f.kind == FRAME_DATA)
			{
				if ((f.seq != frame_expected) && no_nak)
					send_frame_ph(NULL, nak_frame);
				else
					start_ack_timer(ACK_TIMER);
				if (between(frame_expected, f.seq, too_far) && (arrived[f.seq%NR_BUFS] == false))
				//如果收到的帧在接收方窗口内且这一帧未被接收过
				{
					dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short *)f.data);
					arrived[f.seq % NR_BUFS] = true;
					memcpy(in_buffer[f.seq % NR_BUFS], f.data, PKT_LEN);
					while (arrived[frame_expected % NR_BUFS])
					{
						put_packet(in_buffer[frame_expected %NR_BUFS], len - 7); //将输入缓存送至网络层
						no_nak = true;
						arrived[frame_expected % NR_BUFS] = false;
						inc(frame_expected);//将窗口前移一位
						inc(too_far);
						start_ack_timer(ACK_TIMER);
					}
				}
			}

			if ((f.kind == FRAME_NAK) && between(ack_expected, (f.ack + 1) % (MAX_SEQ + 1), next_frame_to_send))
			{
				send_frame_ph((f.ack + 1) % (MAX_SEQ + 1), data_frame);
				dbg_frame("Recv NAK with ACK %d\n", f.ack);
			}

			while (between(ack_expected, f.ack, next_frame_to_send))
			{
				nbuffered--;
				stop_timer(ack_expected%NR_BUFS);
				inc(ack_expected);
			}
			break;
		}

		case ACK_TIMEOUT: {
			dbg_event("---- DATA %d timeout\n", oldest_frame);
			send_frame_ph(NULL, ack_frame);
			break;
		}
		/*
		当出现ACK超时的现象，则向屏幕输出提醒，并发送ACK包；
		*/
		case DATA_TIMEOUT: { //如果收到的帧在接收方窗口内且这一帧未被接收过
			dbg_event("---- DATA %d timeout\n", oldest_frame);

			if (!between(ack_expected, oldest_frame, next_frame_to_send))
				oldest_frame = oldest_frame + NR_BUFS;

			send_frame_ph(oldest_frame, data_frame);
			break;
		}
		/*
		当出现数据包超时的现象，说明对方没有收到数据包，则重新发送数据包
		*/
		}
		if (nbuffered < NR_BUFS&& phl_ready)
			enable_network_layer();
		//当输出缓存没有达到最大值并且物理层可传输时，则运行网络层可用函数
		else
			disable_network_layer();
		//否则，则网络层不可用
	}

}
