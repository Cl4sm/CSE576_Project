static int pcapfile_get_wire_length(const libtrace_packet_t *packet) {
	libtrace_pcapfile_pkt_hdr_t *pcapptr;

	assert(packet->header); 

	pcapptr	= (libtrace_pcapfile_pkt_hdr_t *)packet->header;
	if (packet->type==pcap_linktype_to_rt(TRACE_DLT_EN10MB))
		/* Include the missing FCS */
		return swapl(packet->trace,pcapptr->wirelen)+4; 
	else if (packet->type==pcap_linktype_to_rt(TRACE_DLT_IEEE802_11_RADIO)){
		/* If the packet is Radiotap and the flags field indicates
		 * that the FCS is not included in the 802.11 frame, then
		 * we need to add 4 to the wire-length to account for it.
		 */
		uint8_t flags;
		void *link;
		libtrace_linktype_t linktype;
		link = trace_get_packet_buffer(packet, &linktype, NULL);
		trace_get_wireless_flags(link, linktype, &flags);
		if ((flags & TRACE_RADIOTAP_F_FCS) == 0)
			return swapl(packet->trace,pcapptr->wirelen)+4;
	} else if (packet->type == pcap_linktype_to_rt(TRACE_DLT_LINUX_SLL)) {
		libtrace_sll_header_t *sll;
		sll = (libtrace_sll_header_t *)packet->payload;

		/* Account for FCS when dealing with Ethernet packets that are
		 * encapsulated in Linux SLL. This should fix the problem
		 * where the wire lengths differ if we convert the packet to
		 * ERF */
		if (ntohs(sll->protocol) == TRACE_ETHERTYPE_LOOPBACK)
			return swapl(packet->trace,pcapptr->wirelen)+4;
	}

	return swapl(packet->trace,pcapptr->wirelen);
}