void tcpopt_per_packet(struct libtrace_packet_t *packet)
{
	struct libtrace_tcp *tcp = trace_get_tcp(packet);
	unsigned char *opt_ptr;
	libtrace_direction_t dir = trace_get_direction(packet);
	int tcp_payload, len;
	unsigned char type, optlen, *data;
	
	if(!tcp)
		return;
	
	if (dir != TRACE_DIR_INCOMING && dir != TRACE_DIR_OUTGOING)
		dir = TRACE_DIR_OTHER;
	
	len = tcp->doff * 4 - sizeof(libtrace_tcp_t);
	if(len == 0)
		return;
	
	tcp_payload = trace_get_wire_length(packet) - trace_get_capture_length(packet);
	
	opt_ptr = (unsigned char *)tcp + sizeof (libtrace_tcp_t);
	
	while(trace_get_next_option(&opt_ptr,&len,&type,&optlen,&data)){
		/* I don't think we need to count NO-OPs */
		if (type == 1)
			continue;
		tcpopt_stat[dir][type].count++;
		tcpopt_stat[dir][type].bytes+= tcp_payload;
	}
	
}