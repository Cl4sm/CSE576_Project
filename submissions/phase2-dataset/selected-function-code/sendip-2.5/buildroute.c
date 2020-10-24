	char *data_out = data;
	char *data_in = data;
	char *next;
	u_int8_t p='0';
	int i;
	/* First, the first 2 bytes give us the pointer */
	for(i=0;i<2;i++) {
		p<<=4;
		if('0'<=*data_in && *data_in<='9') {
			p+=*data_in-'0';
		} else if('A'<=*data_in && *data_in<='F') {
			p+=*data_in-'A'+0x0A;
		} else if('a'<=*data_in && *data_in<='f') {
			p+=*data_in-'a'+0x0a;
		} else {
			fprintf(stderr,"First 2 chars of record route options must be hex pointer\n");
			return 0;
		}
		data_in++;
	}
	*(data_out++)=p;

	/* Now loop through IP addresses... */
	if(*data_in != ':') {
		fprintf(stderr,"Third char of a record route option must be a :\n");
		return 0;
	}
	data_in++;
	next=data_in;
	while(next) {
		u_int32_t ip;
		next=strchr(data_in,':');
		if(next) {
			*(next++)=0;
		}
		ip=inet_addr(data_in);
		memcpy(data_out,&ip,4);
		data_out+=4;
		data_in = next;
	}

	return (data_out-data);
}
