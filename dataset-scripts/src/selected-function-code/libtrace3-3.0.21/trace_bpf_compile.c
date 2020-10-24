static int trace_bpf_compile(libtrace_filter_t *filter,
		const libtrace_packet_t *packet,
		void *linkptr, 
		libtrace_linktype_t linktype	) {
#ifdef HAVE_BPF_FILTER
	assert(filter);

	/* If this isn't a real packet, then fail */
	if (!linkptr) {
		trace_set_err(packet->trace,
				TRACE_ERR_BAD_FILTER,"Packet has no payload");
		return -1;
	}
	
	if (filter->filterstring && ! filter->flag) {
		pcap_t *pcap = NULL;
		if (linktype==(libtrace_linktype_t)-1) {
			trace_set_err(packet->trace,
					TRACE_ERR_BAD_FILTER,
					"Packet has an unknown linktype");
			return -1;
		}
		if (libtrace_to_pcap_dlt(linktype) == TRACE_DLT_ERROR) {
			trace_set_err(packet->trace,TRACE_ERR_BAD_FILTER,
					"Unknown pcap equivalent linktype");
			return -1;
		}
		pcap=(pcap_t *)pcap_open_dead(
				(int)libtrace_to_pcap_dlt(linktype),
				1500U);
		/* build filter */
		assert(pcap);
		if (pcap_compile( pcap, &filter->filter, filter->filterstring, 
					1, 0)) {
			trace_set_err(packet->trace,TRACE_ERR_BAD_FILTER,
					"Unable to compile the filter \"%s\": %s", 
					filter->filterstring,
					pcap_geterr(pcap));
			pcap_close(pcap);
			return -1;
		}
		pcap_close(pcap);
		filter->flag=1;
	}
	return 0;
#else
	assert(!"Internal bug: This should never be called when BPF not enabled");
	trace_set_err(packet->trace,TRACE_ERR_OPTION_UNAVAIL,
				"Feature unavailable");
	return -1;
#endif
}