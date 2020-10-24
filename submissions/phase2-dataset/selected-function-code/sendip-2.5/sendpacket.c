							 bool verbose) {
	_sockaddr_storage *to = malloc(sizeof(_sockaddr_storage));
	int tolen;

	/* socket stuff */
	int s;                            /* socket for sending       */

	/* hostname stuff */
	struct hostent *host = NULL;      /* result of gethostbyname2 */

	/* casts for specific protocols */
	struct sockaddr_in *to4 = (struct sockaddr_in *)to; /* IPv4 */
	struct sockaddr_in6 *to6 = (struct sockaddr_in6 *)to; /* IPv6 */

	int sent;                         /* number of bytes sent */

	if(to==NULL) {
		perror("OUT OF MEMORY!\n");
		return -3;
	}
	memset(to, 0, sizeof(_sockaddr_storage));

	if ((host = gethostbyname2(hostname, af_type)) == NULL) {
		perror("Couldn't get destination host: gethostbyname2()");
		free(to);
		return -1;
	}

	switch (af_type) {
	case AF_INET:
		to4->sin_family = host->h_addrtype;
		memcpy(&to4->sin_addr, host->h_addr, host->h_length);
		tolen = sizeof(struct sockaddr_in);
		break;
	case AF_INET6:
		to6->sin6_family = host->h_addrtype;
		memcpy(&to6->sin6_addr, host->h_addr, host->h_length);
		tolen = sizeof(struct sockaddr_in6);
		break;
	default:
		return -2;
		break;
	}

	if(verbose) { 
		int i, j;  
		printf("Final packet data:\n");
		for(i=0; i<data->alloc_len; ) {
			for(j=0; j<4 && i+j<data->alloc_len; j++)
				printf("%02X ", ((unsigned char *)(data->data))[i+j]); 
			printf("  ");
			for(j=0; j<4 && i+j<data->alloc_len; j++) {
				int c=(int) ((unsigned char *)(data->data))[i+j];
				printf("%c", isprint(c)?((char *)(data->data))[i+j]:'.'); 
			}
			printf("\n");
			i+=j;
		}
	}

	if ((s = socket(af_type, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("Couldn't open RAW socket");
		free(to);
		return -1;
	}
	/* Need this for OpenBSD, shouldn't cause problems elsewhere */
	/* TODO: should make it a command line option */
	if(af_type == AF_INET) { 
		const int on=1;
		if (setsockopt(s, IPPROTO_IP,IP_HDRINCL,(const void *)&on,sizeof(on)) <0) { 
			perror ("Couldn't setsockopt IP_HDRINCL");
			free(to);
			close(s);
			return -2;
		}
	}

	/* On Solaris, it seems that the only way to send IP options or packets
		with a faked IP header length is to:
		setsockopt(IP_OPTIONS) with the IP option data and size
		decrease the total length of the packet accordingly
		I'm sure this *shouldn't* work.  But it does.
	*/
#ifdef __sun__
	if((*((char *)(data->data))&0x0F) != 5) {
		ip_header *iphdr = (ip_header *)data->data;

		int optlen = iphdr->header_len*4-20;

		if(verbose) 
			printf("Solaris workaround enabled for %d IP option bytes\n", optlen);

		iphdr->tot_len = htons(ntohs(iphdr->tot_len)-optlen);

		if(setsockopt(s,IPPROTO_IP,IP_OPTIONS,
						  (void *)(((char *)(data->data))+20),optlen)) {
			perror("Couldn't setsockopt IP_OPTIONS");
			free(to);
			close(s);
			return -2;
		}
	}
#endif /* __sun__ */

	/* Send the packet */
	sent = sendto(s, (char *)data->data, data->alloc_len, 0, (void *)to, tolen);
	if (sent == data->alloc_len) {
		if(verbose) printf("Sent %d bytes to %s\n",sent,hostname);
	} else {
		if (sent < 0)
			perror("sendto");
		else {
			if(verbose) fprintf(stderr, "Only sent %d of %d bytes to %s\n", 
									  sent, data->alloc_len, hostname);
		}
	}
	free(to);
	close(s);
	return sent;
}
