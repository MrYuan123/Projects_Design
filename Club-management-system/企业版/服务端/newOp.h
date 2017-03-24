void sort_alg(club s,int *preSign,string str,int kind){
	
	int sign[s.get_playerSum()];
	for(int i=0;i<s.get_playerSum();i++){
		sign[i]=1;	
	}
	
	char t;
	int m=0;    //用于标记当前读取字符串所在的位置 
	int   signOp;     //用于标记返回值做什么运算 
	
	cout<<"Please input the demand:"<<endl;
	fflush(stdin);
	getline(cin,str); 
	deal_space(str);
//////////////////////////////////////////
	t=str[m];
	if(t=='('){
		int leftB=1,rightB=0,nowB=1;
		t=str[nowB];
		while(t!=NULL){
			if(t=='(')
				leftB++;
			else if(t==')')
				rightB++;
				
			cout<<rightB<<" "<<rightB<<endl;
			nowB++;
			t=str[nowB];
			if(leftB==rightB){
			cout<<"ok"<<endl;
			fflush(stdin);
			string goAhead;
			goAhead=str.substr(1,nowB-2);
			sort_player(s,sign,goAhead,1);	
			}
		}
	}
	else if(t=='&'){
		sort_player(s,sign,str.substr(2,nowB-1),1);
	}
	
	else if (t=='|'){
		sort_player(s,sign,str.substr(2,nowB-1),2);
	}	
	
	else{
		
	} 
} 

#endif
