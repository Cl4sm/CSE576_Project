static bool find_compatible_linktype(libtrace_out_t *libtrace,
				libtrace_packet_t *packet)
{
	/* Keep trying to simplify the packet until we can find 
	 * something we can do with it */
	do {
		char type=libtrace_to_erf_type(trace_get_link_type(packet));

		/* Success */
		if (type != (char)-1)
			return true;

		if (!demote_packet(packet)) {
			trace_set_err_out(libtrace,
					TRACE_ERR_NO_CONVERSION,
					"No erf type for packet (%i)",
					trace_get_link_type(packet));
			return false;
		}

	} while(1);

	return true;
}