static int dag_get_padding(const libtrace_packet_t *packet)
{
	/* ERF Ethernet records have a 2 byte padding before the packet itself
	 * so that the IP header is aligned on a 32 bit boundary.
	 */
	if (packet->trace->format->type==TRACE_FORMAT_ERF) {
		dag_record_t *erfptr = (dag_record_t *)packet->header;
		switch(erfptr->type) {
			case TYPE_ETH:
			case TYPE_DSM_COLOR_ETH:
				return 2;
			default: 		return 0;
		}
	}
	else {
		switch(trace_get_link_type(packet)) {
			case TRACE_TYPE_ETH:	return 2;
			default:		return 0;
		}
	}
}