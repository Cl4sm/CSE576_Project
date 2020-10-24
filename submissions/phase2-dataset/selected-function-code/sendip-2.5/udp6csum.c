							sendip_data *data) {
	udp_header *udp = (udp_header *)udp_hdr->data;
	ipv6_header  *ipv6  = (ipv6_header *)ipv6_hdr->data;
	struct ipv6_pseudo_hdr phdr;

	u_int16_t *buf = malloc(sizeof(phdr)+udp_hdr->alloc_len+data->alloc_len);
	u_int8_t *tempbuf = (u_int8_t *)buf;
	udp->check=0;
	if(tempbuf == NULL) {
		fprintf(stderr,"Out of memory: UDP checksum not computed\n");
		return;
	}

	/* Set up the pseudo header */
	memset(&phdr,0,sizeof(phdr));
	memcpy(&phdr.source,&ipv6->ip6_src,sizeof(struct in6_addr));
	memcpy(&phdr.destination,&ipv6->ip6_dst,sizeof(struct in6_addr));
	phdr.ulp_length=IPPROTO_UDP;
	
	memcpy(tempbuf,&phdr,sizeof(phdr));

	/* Copy the UDP header and data */
	memcpy(tempbuf+sizeof(phdr),udp_hdr->data,udp_hdr->alloc_len);
	memcpy(tempbuf+sizeof(phdr)+udp_hdr->alloc_len,data->data,data->alloc_len);

	/* CheckSum it */
	udp->check = csum(buf,sizeof(phdr)+udp_hdr->alloc_len+data->alloc_len);
	free(buf);
}
