static void icmp6csum(struct in6_addr *src, struct in6_addr *dst,
							 sendip_data *hdr, sendip_data *data) {
	icmp_header *icp = (icmp_header *)hdr->data;
	struct ipv6_pseudo_hdr phdr;

	/* Make sure tempbuf is word aligned */
	u_int16_t *buf = malloc(sizeof(phdr)+hdr->alloc_len+data->alloc_len);
	u_int8_t *tempbuf = (u_int8_t *)buf;
	icp->check = 0;
	if(tempbuf == NULL) {
		fprintf(stderr,"Out of memory: ICMP checksum not computed\n");
		return;
	}
	memcpy(tempbuf+sizeof(phdr), hdr->data, hdr->alloc_len);
	memcpy(tempbuf+sizeof(phdr)+hdr->alloc_len, data->data, data->alloc_len);

	/* do an ipv6 checksum */
	memset(&phdr, 0, sizeof(phdr));
	memcpy(&phdr.source, src, sizeof(struct in6_addr));
	memcpy(&phdr.destination, dst, sizeof(struct in6_addr));
	phdr.ulp_length = htonl(hdr->alloc_len+data->alloc_len);
	phdr.nexthdr = IPPROTO_ICMPV6;
	
	memcpy(tempbuf, &phdr, sizeof(phdr));
	
	icp->check = csum(buf,sizeof(phdr)+hdr->alloc_len+data->alloc_len);
	free(buf);
}
