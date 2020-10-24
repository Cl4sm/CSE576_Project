void port_per_packet(struct libtrace_packet_t *packet)
{
	uint8_t proto;
	int port;
	libtrace_direction_t dir = trace_get_direction(packet);

	if(trace_get_transport(packet,&proto,NULL)==NULL) 
		return;

	if (dir != TRACE_DIR_INCOMING && dir != TRACE_DIR_OUTGOING)
		dir = TRACE_DIR_OTHER;
	
	port = trace_get_server_port(proto,
			trace_get_source_port(packet),
			trace_get_destination_port(packet))==USE_SOURCE
		? trace_get_source_port(packet)
		: trace_get_destination_port(packet);

	if (!ports[dir][proto])
		ports[dir][proto]=calloc(65536,sizeof(stat_t));
	ports[dir][proto][port].bytes+=trace_get_wire_length(packet);
	ports[dir][proto][port].count++;
	protn[proto]=1;
	suppress[dir] = false;
}