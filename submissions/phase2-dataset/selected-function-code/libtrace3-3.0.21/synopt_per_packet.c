void synopt_per_packet(struct libtrace_packet_t *packet)
{
	struct libtrace_tcp *tcp = trace_get_tcp(packet);
	unsigned char *opt_ptr;
	libtrace_direction_t dir = trace_get_direction(packet);
	int len;
	unsigned char type, optlen, *data;
	struct tcp_opts opts_seen = {false, false, false, false, false, false};
	
	if(!tcp)
		return;

	if (!tcp->syn)
		return;
	
	if (dir != TRACE_DIR_INCOMING && dir != TRACE_DIR_OUTGOING)
		dir = TRACE_DIR_OTHER;
	
	len = tcp->doff * 4 - sizeof(libtrace_tcp_t);
	if(len == 0)
		return;
	
	opt_ptr = (unsigned char *)tcp + sizeof (libtrace_tcp_t);
	
	while(trace_get_next_option(&opt_ptr,&len,&type,&optlen,&data)){
		/* I don't think we need to count NO-OPs */
		if (type == 1)
			continue;
		switch(type) {
			case 2:
				opts_seen.mss = true;
				break;
			case 3:
				opts_seen.winscale = true;
				break;
			case 4:
				opts_seen.sack = true;
				break;
			case 5:
				opts_seen.sack = true;
				break;
			case 8:
				opts_seen.ts = true;
				break;
			case 11:
			case 12:
			case 13:
				opts_seen.ttcp = true;
				break;
			default:
				opts_seen.other = true;
		}
	}

	if (tcp->ack) {
		total_synacks ++;
		classify_packet(opts_seen, &synack_counts);
	} else {
		total_syns ++;
		classify_packet(opts_seen, &syn_counts);
	}
}