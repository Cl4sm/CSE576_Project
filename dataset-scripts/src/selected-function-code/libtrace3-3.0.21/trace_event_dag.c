static libtrace_eventobj_t trace_event_dag(libtrace_t *trace,
                                        libtrace_packet_t *packet) {
        libtrace_eventobj_t event = {0,0,0.0,0};
        int data;

	do {
	        data = dag_available(trace);

		/* If no data is available, drop out and return a sleep event */
		if (data <= 0)
			break;

		/* Data is available, so we can call the blocking read because
		 * we know that we will get a packet straight away */
                event.size = dag_read_packet(trace,packet);
                //DATA(trace)->dag.diff -= event.size;
		
		/* XXX trace_read_packet() normally applies the following
		 * config options for us, but this function is called via
		 * trace_event() so we have to do it ourselves */

		/* Check that the packet matches any pre-existing filter */
                if (trace->filter) {
                        if (trace_apply_filter(trace->filter, packet)) {
                                event.type = TRACE_EVENT_PACKET;
                        } else {
                                /* Do not sleep - try to read another packet */
                                trace->filtered_packets ++;
				continue;
                        }
                } else {
                        event.type = TRACE_EVENT_PACKET;
                }

		/* If the user has specified a snap length, apply that too */
                if (trace->snaplen > 0) {
                        trace_set_capture_length(packet, trace->snaplen);
                }
                trace->accepted_packets ++;
                return event;
        } while (1);


	/* We only want to sleep for a very short time */
        assert(data == 0);
        event.type = TRACE_EVENT_SLEEP;
        event.seconds = 0.0001;
        event.size = 0;
	return event;
}