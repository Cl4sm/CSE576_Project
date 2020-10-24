							sendip_data *data) {
	tcp_header *tcp = (tcp_header *)tcp_hdr->data;
	ipv6_header  *ipv6  = (ipv6_header *)ipv6_hdr->data;
	struct ipv6_pseudo_hdr phdr;

	u_int16_t *buf = malloc(sizeof(phdr)+tcp_hdr->alloc_len+data->alloc_len);
	u_int8_t *tempbuf = (u_int8_t *)buf;
	tcp->check=0;
	if(tempbuf == NULL) {
		fprintf(stderr,"Out of memory: TCP checksum not computed\n");
		return;
	}

	/* Set up the pseudo header */
	memset(&phdr,0,sizeof(phdr));
	memcpy(&phdr.source,&ipv6->ip6_src,sizeof(struct in6_addr));
	memcpy(&phdr.destination,&ipv6->ip6_dst,sizeof(struct in6_addr));
	phdr.ulp_length=IPPROTO_TCP;
	
	memcpy(tempbuf,&phdr,sizeof(phdr));

	/* Copy the TCP header and data */
	memcpy(tempbuf+sizeof(phdr),tcp_hdr->data,tcp_hdr->alloc_len);
	memcpy(tempbuf+sizeof(phdr)+tcp_hdr->alloc_len,data->data,data->alloc_len);

	/* CheckSum it */
	tcp->check = csum(buf,sizeof(phdr)+tcp_hdr->alloc_len+data->alloc_len);
	free(buf);
}
