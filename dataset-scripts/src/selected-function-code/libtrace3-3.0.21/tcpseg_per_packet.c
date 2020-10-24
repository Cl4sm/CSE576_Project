void tcpseg_per_packet(struct libtrace_packet_t *packet)
{
	struct libtrace_tcp *tcp = trace_get_tcp(packet);
	libtrace_ip_t *ip = trace_get_ip(packet);
	libtrace_direction_t dir = trace_get_direction(packet);
	int ss;
	uint16_t ip_len ;
	
	if (!tcp || !ip)
		return;

	if (dir != TRACE_DIR_INCOMING && dir != TRACE_DIR_OUTGOING)
		dir = TRACE_DIR_OTHER;
	
	ip_len = ntohs(ip->ip_len);
	ss = ip_len - (ip->ip_hl * 4);

	if (ss > MAX_SEG_SIZE) {
		fprintf(stderr, "Maximum segment size %u exceeded - size was %u\n",
				MAX_SEG_SIZE, ss);
		return;
	}


	tcpseg_stat[dir][ss].count++;
	tcpseg_stat[dir][ss].bytes+=trace_get_wire_length(packet);
	suppress[dir] = false;
}