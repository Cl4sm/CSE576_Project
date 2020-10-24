void trace_construct_packet(libtrace_packet_t *packet,
		libtrace_linktype_t linktype,
		const void *data,
		uint16_t len)
{
	size_t size;
	static libtrace_t *deadtrace=NULL;
	libtrace_pcapfile_pkt_hdr_t hdr;
#ifdef WIN32
	struct _timeb tstruct;
#else
	struct timeval tv;
#endif

	/* We need a trace to attach the constructed packet to (and it needs
	 * to be PCAP) */
	if (NULL == deadtrace) 
		deadtrace=trace_create_dead("pcapfile");

	/* Fill in the new PCAP header */
#ifdef WIN32
	_ftime(&tstruct);
	hdr.ts_sec=tstruct.time;
	hdr.ts_usec=tstruct.millitm * 1000;
#else
	gettimeofday(&tv,NULL);
	hdr.ts_sec=tv.tv_sec;
	hdr.ts_usec=tv.tv_usec;
#endif

	hdr.caplen=len;
	hdr.wirelen=len;

	/* Now fill in the libtrace packet itself */
	packet->trace=deadtrace;
	size=len+sizeof(hdr);
	if (packet->buf_control==TRACE_CTRL_PACKET) {
		packet->buffer=realloc(packet->buffer,size);
	}
	else {
		packet->buffer=malloc(size);
	}
	packet->buf_control=TRACE_CTRL_PACKET;
	packet->header=packet->buffer;
	packet->payload=(void*)((char*)packet->buffer+sizeof(hdr));
	
	/* Ugh, memcpy - sadly necessary */
	memcpy(packet->header,&hdr,sizeof(hdr));
	memcpy(packet->payload,data,(size_t)len);
	packet->type=pcap_linktype_to_rt(libtrace_to_pcap_linktype(linktype));

	trace_clear_cache(packet);
}