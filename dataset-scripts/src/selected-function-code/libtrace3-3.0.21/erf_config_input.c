static int erf_config_input(libtrace_t *libtrace, trace_option_t option,
		void *value) {

	switch (option) {
		case TRACE_OPTION_EVENT_REALTIME:
			IN_OPTIONS.real_time = *(int *)value;
			return 0;
		case TRACE_OPTION_SNAPLEN:
		case TRACE_OPTION_PROMISC:
		case TRACE_OPTION_FILTER:
		case TRACE_OPTION_META_FREQ:
			trace_set_err(libtrace, TRACE_ERR_OPTION_UNAVAIL,
					"Unsupported option");
			return -1;
		default:
			/* Unknown option */
			trace_set_err(libtrace,TRACE_ERR_UNKNOWN_OPTION,
					"Unknown option");
			return -1;
	}
}