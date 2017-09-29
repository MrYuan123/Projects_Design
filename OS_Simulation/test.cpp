else{ //空闲区长度大于进程所需空间，从首地址开始查询，找到大于进程长度的及分配给进程
	t=0; 
	i=0; 
	while(t==0){ 
		if(frees[i].length>=job_length){ 
			t=1; 	} 
	i++; } 
	i--; 
	occupys[occupy_quantity].start=frees[i].start; 
	strcpy(occupys[occupy_quantity].tag,job_name); 
	occupys[occupy_quantity].length=job_length; 
	occupy_quantity++; 
	if(frees[i].length>job_length){ 
		frees[i].start+=job_length; 
		frees[i].length-=job_length; } 
else{ 
	for(j=i;j<free_quantity-1;j++){ 
		frees[j]=frees[j+1]; } 
	free_quantity--;
