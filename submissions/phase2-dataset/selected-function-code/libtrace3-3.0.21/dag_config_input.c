static int dag_config_input(libtrace_t *libtrace, trace_option_t option,
                                void *data) {
        switch(option) {
                case TRACE_OPTION_META_FREQ:
			/* We use this option to specify the frequency of
			 * DUCK updates */
                        DUCK.duck_freq = *(int *)data;
                        return 0;
                case TRACE_OPTION_SNAPLEN:
                        /* Surely we can set this?? Fall through for now*/
                        return -1;
                case TRACE_OPTION_PROMISC:
                        /* DAG already operates in a promisc fashion */
                        return -1;
                case TRACE_OPTION_FILTER:
			/* Cards that use the older drivers don't do 
			 * filtering */
                        return -1;
		case TRACE_OPTION_EVENT_REALTIME:
			/* Live capture is always going to be realtime */
			return -1;
        }
	return -1;
}