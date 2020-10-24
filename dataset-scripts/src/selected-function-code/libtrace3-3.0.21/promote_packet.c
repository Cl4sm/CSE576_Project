void promote_packet(libtrace_packet_t *packet)
{
	if (packet->trace->format->type == TRACE_FORMAT_PCAP) {
		char *tmpbuffer;
		libtrace_sll_header_t *hdr;

		if (pcap_linktype_to_libtrace(rt_to_pcap_linktype(packet->type))
			== TRACE_TYPE_LINUX_SLL) {
			/* This is already been promoted, so ignore it */
			return;
		}

		/* This should be easy, just prepend the header */
		tmpbuffer= (char*)malloc(
				sizeof(libtrace_sll_header_t)
				+trace_get_capture_length(packet)
				+trace_get_framing_length(packet)
				);

		hdr=(libtrace_sll_header_t*)((char*)tmpbuffer
			+trace_get_framing_length(packet));

		hdr->halen=htons(6);
		hdr->pkttype=TRACE_SLL_OUTGOING;

		switch(pcap_linktype_to_libtrace(rt_to_pcap_linktype(packet->type))) {
			case TRACE_TYPE_NONE:
				trace_get_layer3(packet, &hdr->protocol, NULL);
				hdr->hatype = htons(LIBTRACE_ARPHRD_PPP);
				hdr->protocol=htons(hdr->protocol);
				break;
			case TRACE_TYPE_ETH:
				hdr->hatype = htons(LIBTRACE_ARPHRD_ETHER);
				hdr->protocol=htons(0x0060); /* ETH_P_LOOP */
				break;
			default:
				/* failed */
				return;
		}
		memcpy(tmpbuffer,packet->header,
				trace_get_framing_length(packet));
		memcpy(tmpbuffer
				+sizeof(libtrace_sll_header_t)
				+trace_get_framing_length(packet),
				packet->payload,
				trace_get_capture_length(packet));
		if (packet->buf_control == TRACE_CTRL_EXTERNAL) {
			packet->buf_control=TRACE_CTRL_PACKET;
		}
		else {
			free(packet->buffer);
		}
		packet->buffer=tmpbuffer;
		packet->header=tmpbuffer;
		packet->payload=tmpbuffer+trace_get_framing_length(packet);
		packet->type=pcap_linktype_to_rt(TRACE_DLT_LINUX_SLL);
		((struct libtrace_pcapfile_pkt_hdr_t*) packet->header)->caplen+=
			sizeof(libtrace_sll_header_t);
		((struct libtrace_pcapfile_pkt_hdr_t*) packet->header)->wirelen+=
			sizeof(libtrace_sll_header_t);
		trace_clear_cache(packet);
		return;
	}
}