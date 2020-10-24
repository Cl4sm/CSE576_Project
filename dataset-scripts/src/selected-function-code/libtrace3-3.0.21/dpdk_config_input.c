static int dpdk_config_input (libtrace_t *libtrace,
                                        trace_option_t option,
                                        void *data) {
    switch (option) {
        case TRACE_OPTION_SNAPLEN:
            /* Only support changing snaplen before a call to start is 
             * made */
            if (FORMAT(libtrace)->paused == DPDK_NEVER_STARTED)
                FORMAT(libtrace)->snaplen=*(int*)data;
            else
                return -1;
            return 0;
		case TRACE_OPTION_PROMISC:
			FORMAT(libtrace)->promisc=*(int*)data;
            return 0;
        case TRACE_OPTION_FILTER:
            /* TODO filtering */
            break;
        case TRACE_OPTION_META_FREQ:
            break;
        case TRACE_OPTION_EVENT_REALTIME:
            break;
        /* Avoid default: so that future options will cause a warning
         * here to remind us to implement it, or flag it as
         * unimplementable
         */
    }

	/* Don't set an error - trace_config will try to deal with the
	 * option and will set an error if it fails */
    return -1;
}