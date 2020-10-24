static int pcapfile_config_input(libtrace_t *libtrace,
		trace_option_t option,
		void *data)
{
	switch(option) {
		case TRACE_OPTION_EVENT_REALTIME:
			IN_OPTIONS.real_time = *(int *)data;
			return 0;
		case TRACE_OPTION_META_FREQ:
		case TRACE_OPTION_SNAPLEN:
		case TRACE_OPTION_PROMISC:
		case TRACE_OPTION_FILTER:
			/* All these are either unsupported or handled
			 * by trace_config */
			break;
	}
	
	trace_set_err(libtrace,TRACE_ERR_UNKNOWN_OPTION,
			"Unknown option %i", option);
	return -1;
}