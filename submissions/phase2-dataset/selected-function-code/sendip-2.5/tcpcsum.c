						  sendip_data *data) {
	tcp_header *tcp = (tcp_header *)tcp_hdr->data;
	ip_header  *ip  = (ip_header *)ip_hdr->data;
	u_int16_t *buf = malloc(12+tcp_hdr->alloc_len+data->alloc_len);
	u_int8_t *tempbuf = (u_int8_t *)buf;
	tcp->check=0;
	if(tempbuf == NULL) {
		fprintf(stderr,"Out of memory: TCP checksum not computed\n");
		return;
	}
	/* Set up the pseudo header */
	memcpy(tempbuf,&(ip->saddr),sizeof(u_int32_t));
	memcpy(&(tempbuf[4]),&(ip->daddr),sizeof(u_int32_t));
	tempbuf[8]=0;
	tempbuf[9]=(u_int16_t)ip->protocol;
	tempbuf[10]=(u_int16_t)((tcp_hdr->alloc_len+data->alloc_len)&0xFF00)>>8;
	tempbuf[11]=(u_int16_t)((tcp_hdr->alloc_len+data->alloc_len)&0x00FF);
	/* Copy the TCP header and data */
	memcpy(tempbuf+12,tcp_hdr->data,tcp_hdr->alloc_len);
	memcpy(tempbuf+12+tcp_hdr->alloc_len,data->data,data->alloc_len);
	/* CheckSum it */
	tcp->check = csum(buf,12+tcp_hdr->alloc_len+data->alloc_len);
	free(buf);
}
