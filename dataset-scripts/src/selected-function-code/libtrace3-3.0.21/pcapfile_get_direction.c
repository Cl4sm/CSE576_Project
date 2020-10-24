static libtrace_direction_t pcapfile_get_direction(const libtrace_packet_t *packet) 
{
	libtrace_direction_t direction  = -1;
	switch(pcapfile_get_link_type(packet)) {
		/* We can only get the direction for PCAP packets that have
		 * been encapsulated in Linux SLL or PFLOG */
		case TRACE_TYPE_LINUX_SLL:
		{
			libtrace_sll_header_t *sll;
			libtrace_linktype_t linktype;

			sll = (libtrace_sll_header_t*)trace_get_packet_buffer(
					packet,
					&linktype,
					NULL);
			if (!sll) {
				trace_set_err(packet->trace,
					TRACE_ERR_BAD_PACKET,
						"Bad or missing packet");
				return -1;
			}
			/* 0 == LINUX_SLL_HOST */
			/* the Waikato Capture point defines "packets
			 * originating locally" (ie, outbound), with a
			 * direction of 0, and "packets destined locally"
			 * (ie, inbound), with a direction of 1.
			 * This is kind-of-opposite to LINUX_SLL.
			 * We return consistent values here, however
			 *
			 * Note that in recent versions of pcap, you can
			 * use "inbound" and "outbound" on ppp in linux
			 */
			if (ntohs(sll->pkttype == 0)) {
				direction = TRACE_DIR_INCOMING;
			} else {
				direction = TRACE_DIR_OUTGOING;
			}
			break;

		}
		case TRACE_TYPE_PFLOG:
		{
			libtrace_pflog_header_t *pflog;
			libtrace_linktype_t linktype;

			pflog=(libtrace_pflog_header_t*)trace_get_packet_buffer(
					packet,&linktype,NULL);
			if (!pflog) {
				trace_set_err(packet->trace,
						TRACE_ERR_BAD_PACKET,
						"Bad or missing packet");
				return -1;
			}
			/* enum    { PF_IN=0, PF_OUT=1 }; */
			if (ntohs(pflog->dir==0)) {

				direction = TRACE_DIR_INCOMING;
			}
			else {
				direction = TRACE_DIR_OUTGOING;
			}
			break;
		}
		default:
			break;
	}	
	return direction;
}