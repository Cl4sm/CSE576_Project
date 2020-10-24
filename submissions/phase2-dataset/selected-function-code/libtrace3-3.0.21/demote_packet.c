bool demote_packet(libtrace_packet_t *packet)
{
	uint8_t type;
	uint16_t ha_type, next_proto;
	libtrace_sll_header_t *sll = NULL;
	uint32_t remaining = 0;
	char *tmp;
	struct timeval tv;
	static libtrace_t *trace = NULL;
	switch(trace_get_link_type(packet)) {
		case TRACE_TYPE_ATM:
			remaining=trace_get_capture_length(packet);
			packet->payload=trace_get_payload_from_atm(
				packet->payload,&type,&remaining);
			if (!packet->payload)
				return false;
			tmp=(char*)malloc(
				trace_get_capture_length(packet)
				+sizeof(libtrace_pcapfile_pkt_hdr_t)
				);

			tv=trace_get_timeval(packet);
			((libtrace_pcapfile_pkt_hdr_t*)tmp)->ts_sec=tv.tv_sec;
			((libtrace_pcapfile_pkt_hdr_t*)tmp)->ts_usec=tv.tv_usec;
			((libtrace_pcapfile_pkt_hdr_t*)tmp)->wirelen
				= trace_get_wire_length(packet)-(trace_get_capture_length(packet)-remaining);
			((libtrace_pcapfile_pkt_hdr_t*)tmp)->caplen
				= remaining;

			memcpy(tmp+sizeof(libtrace_pcapfile_pkt_hdr_t),
					packet->payload,
					(size_t)remaining);
			if (packet->buf_control == TRACE_CTRL_EXTERNAL) {
				packet->buf_control=TRACE_CTRL_PACKET;
			}
			else {
				free(packet->buffer);
			}
			packet->buffer=tmp;
			packet->header=tmp;
			packet->payload=tmp+sizeof(libtrace_pcapfile_pkt_hdr_t);
			packet->type=pcap_linktype_to_rt(TRACE_DLT_ATM_RFC1483);
			
			if (trace == NULL) {
				trace = trace_create_dead("pcapfile:-");
			}

			packet->trace=trace;

			/* Invalidate caches */
			trace_clear_cache(packet);
			return true;

		case TRACE_TYPE_LINUX_SLL:
			sll = (libtrace_sll_header_t *)(packet->payload);

			ha_type = ntohs(sll->hatype);
			next_proto = ntohs(sll->protocol);
		
			/* Preserved from older libtrace behaviour */
			if (ha_type == LIBTRACE_ARPHRD_PPP)
				packet->type = pcap_linktype_to_rt(TRACE_DLT_RAW);
			/* Don't decide trace type based on ha_type,
			 * decide based on the protocol header that is
			 * coming up!
			 */
			else if (next_proto == TRACE_ETHERTYPE_LOOPBACK)
				packet->type = pcap_linktype_to_rt(TRACE_DLT_EN10MB);
			else if (next_proto == TRACE_ETHERTYPE_IP) 
				packet->type = pcap_linktype_to_rt(TRACE_DLT_RAW);
			else if (next_proto == TRACE_ETHERTYPE_IPV6)
				packet->type = pcap_linktype_to_rt(TRACE_DLT_RAW);
			else
				return false;

			/* Skip the Linux SLL header */
			packet->payload=(void*)((char*)packet->payload
					+sizeof(libtrace_sll_header_t));
			trace_set_capture_length(packet,
				trace_get_capture_length(packet)
					-sizeof(libtrace_sll_header_t));

			/* Invalidate caches */
			trace_clear_cache(packet);
			break;
		default:
			return false;
	}

	/* Invalidate caches */
	trace_clear_cache(packet);
	return true;
}