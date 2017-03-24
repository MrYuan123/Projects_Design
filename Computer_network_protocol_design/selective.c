#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#include "protocol.h"
#include "datalink.h"

enum { data_frame = 1, ack_frame, nak_frame };     //�����жϴ�֡����
#define DATA_TIMER   3400
#define ACK_TIMER    900

#define MAX_SEQ 31 
#define NR_BUFS 16    
#define inc(k) if(k < MAX_SEQ)k++;else k=0   //���ڼ�¼���ʹ����Ͻ�

typedef struct FRAME {
	unsigned char kind;    
	unsigned char ack;
	unsigned char seq;
	unsigned char data[PKT_LEN];
	unsigned int padding;
} *Ptr_Frame, FRAME;
/*
next_frame_to_send ��¼��һ����Ҫ���͵İ������
frame_expected ��¼��һ�����յİ������
ack_expected ��¼��Ҫ���ܵ�ACK�����
out_buffer[NR_BUFS][PKT_LEN] ����Ŀǰ���͵İ�
in_buffer[NR_BUFS][PKT_LEN]  ������ܵİ�
nbuffered ��¼Ŀǰ��������еĸ���
phl_ready ��¼������Ƿ���Է�������
no_nak  ���ڼ�¼�Ƿ��Ѿ�����NAK�����ѷ��ͣ������ظ�����
arrived[NR_BUFS] ���ڱ�Ǹ�֡�Ƿ��Ѿ��յ�
too_far ���Ŀǰ���յ�����Ͻ�
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
		��Ϊ�����ȴ�ʱ��ʱ��˵����������㷢��֡����������ȴ�������ȡ���ݣ��洢�ڻ����У�������������
		�������󣬲�����send_frame_ph()����������㷢�����ݣ������洢��֡�������ݵ���ֵ���� 1��
		*/

		case PHYSICAL_LAYER_READY: {
			phl_ready = 1;
			break;
		}
		/*
		��Ϊ�����ȴ�ʱ��ʱ����phl_ready��Ϊ1,��ʾ��ǰ����㴫����ã�
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
				//����յ���֡�ڽ��շ�����������һ֡δ�����չ�
				{
					dbg_frame("Recv DATA %d %d, ID %d\n", f.seq, f.ack, *(short *)f.data);
					arrived[f.seq % NR_BUFS] = true;
					memcpy(in_buffer[f.seq % NR_BUFS], f.data, PKT_LEN);
					while (arrived[frame_expected % NR_BUFS])
					{
						put_packet(in_buffer[frame_expected %NR_BUFS], len - 7); //�����뻺�����������
						no_nak = true;
						arrived[frame_expected % NR_BUFS] = false;
						inc(frame_expected);//������ǰ��һλ
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
		������ACK��ʱ������������Ļ������ѣ�������ACK����
		*/
		case DATA_TIMEOUT: { //����յ���֡�ڽ��շ�����������һ֡δ�����չ�
			dbg_event("---- DATA %d timeout\n", oldest_frame);

			if (!between(ack_expected, oldest_frame, next_frame_to_send))
				oldest_frame = oldest_frame + NR_BUFS;

			send_frame_ph(oldest_frame, data_frame);
			break;
		}
		/*
		���������ݰ���ʱ������˵���Է�û���յ����ݰ��������·������ݰ�
		*/
		}
		if (nbuffered < NR_BUFS&& phl_ready)
			enable_network_layer();
		//���������û�дﵽ���ֵ���������ɴ���ʱ���������������ú���
		else
			disable_network_layer();
		//����������㲻����
	}

}
