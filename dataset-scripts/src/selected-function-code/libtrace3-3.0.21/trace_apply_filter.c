DLLEXPORT int trace_apply_filter(libtrace_filter_t *filter,
			const libtrace_packet_t *packet) {
#ifdef HAVE_BPF_FILTER
	void *linkptr = 0;
	uint32_t clen = 0;
	bool free_packet_needed = false;
	int ret;
	libtrace_linktype_t linktype;
	libtrace_packet_t *packet_copy = (libtrace_packet_t*)packet;

	assert(filter);
	assert(packet);

	/* Match all non-data packets as we probably want them to pass
	 * through to the caller */
	linktype = trace_get_link_type(packet);

	if (linktype == TRACE_TYPE_NONDATA)
		return 1;	

	if (libtrace_to_pcap_dlt(linktype)==TRACE_DLT_ERROR) {
		
		/* If we cannot get a suitable DLT for the packet, it may
		 * be because the packet is encapsulated in a link type that
		 * does not correspond to a DLT. Therefore, we should try
		 * popping off headers until we either can find a suitable
		 * link type or we can't do any more sensible decapsulation. */
		
		/* Copy the packet, as we don't want to trash the one we
		 * were passed in */
		packet_copy=trace_copy_packet(packet);
		free_packet_needed=true;

		while (libtrace_to_pcap_dlt(linktype) == TRACE_DLT_ERROR) {
			if (!demote_packet(packet_copy)) {
				trace_set_err(packet->trace, 
						TRACE_ERR_NO_CONVERSION,
						"pcap does not support this format");
				if (free_packet_needed) {
					trace_destroy_packet(packet_copy);
				}
				return -1;
			}
			linktype = trace_get_link_type(packet_copy);
		}

	}
	
	linkptr = trace_get_packet_buffer(packet_copy,NULL,&clen);
	if (!linkptr) {
		if (free_packet_needed) {
			trace_destroy_packet(packet_copy);
		}
		return 0;
	}

	/* We need to compile the filter now, because before we didn't know 
	 * what the link type was
	 */
	if (trace_bpf_compile(filter,packet_copy,linkptr,linktype)==-1) {
		if (free_packet_needed) {
			trace_destroy_packet(packet_copy);
		}
		return -1;
	}

	/* If we're jitting, we may need to JIT the BPF code now too */
#if HAVE_LLVM
	if (!filter->jitfilter) {
		filter->jitfilter = compile_program(filter->filter.bf_insns, filter->filter.bf_len);
	}
#endif

	assert(filter->flag);
	/* Now execute the filter */
#if HAVE_LLVM
	ret=filter->jitfilter->bpf_run((unsigned char *)linkptr, clen);
#else
	ret=bpf_filter(filter->filter.bf_insns,(u_char*)linkptr,(unsigned int)clen,(unsigned int)clen);
#endif

	/* If we copied the packet earlier, make sure that we free it */
	if (free_packet_needed) {
		trace_destroy_packet(packet_copy);
	}
	return ret;
#else
	fprintf(stderr,"This version of libtrace does not have bpf filter support\n");
	return 0;
#endif
}