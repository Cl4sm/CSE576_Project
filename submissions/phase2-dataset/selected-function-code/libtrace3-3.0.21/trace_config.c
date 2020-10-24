DLLEXPORT int trace_config(libtrace_t *libtrace,
		trace_option_t option,
		void *value)
{
	int ret;

	if (trace_is_err(libtrace)) {
		return -1;
	}
	
	/* If the capture format supports configuration, try using their
	 * native configuration first */
	if (libtrace->format->config_input) {
		ret=libtrace->format->config_input(libtrace,option,value);
		if (ret==0)
			return 0;
	}

	/* If we get here, either the native configuration failed or the
	 * format did not support configuration. However, libtrace can 
	 * deal with some options itself, so give that a go */
	switch(option) {
		case TRACE_OPTION_SNAPLEN:
			/* Clear the error if there was one */
			if (trace_is_err(libtrace)) {
				trace_get_err(libtrace);
			}
			if (*(int*)value<0 
				|| *(int*)value>LIBTRACE_PACKET_BUFSIZE) {
				trace_set_err(libtrace,TRACE_ERR_BAD_STATE,
					"Invalid snap length");
			}
			libtrace->snaplen=*(int*)value;
			return 0;
		case TRACE_OPTION_FILTER:
			/* Clear the error if there was one */
			if (trace_is_err(libtrace)) {
				trace_get_err(libtrace);
			}
			libtrace->filter=(libtrace_filter_t *)value;
			return 0;
		case TRACE_OPTION_PROMISC:
			if (!trace_is_err(libtrace)) {
				trace_set_err(libtrace,TRACE_ERR_OPTION_UNAVAIL,
						"Promisc mode is not supported by this format module");
			}
			return -1;
		case TRACE_OPTION_META_FREQ:
			if (!trace_is_err(libtrace)) {
				trace_set_err(libtrace, 
						TRACE_ERR_OPTION_UNAVAIL,
						"This format does not support meta-data gathering");
			}
			return -1;
		case TRACE_OPTION_EVENT_REALTIME:
			if (!trace_is_err(libtrace)) {
				trace_set_err(libtrace, 
						TRACE_ERR_OPTION_UNAVAIL,
						"This format does not support realtime events");
			}
			return -1;
			
	}
	if (!trace_is_err(libtrace)) {
		trace_set_err(libtrace,TRACE_ERR_UNKNOWN_OPTION,
			"Unknown option %i", option);
	}
	return -1;
}