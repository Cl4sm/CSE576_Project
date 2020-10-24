struct libtrace_eventobj_t trace_event_trace(struct libtrace_t *trace, struct libtrace_packet_t *packet) {
	struct libtrace_eventobj_t event = {0,0,0.0,0};
	double ts;
	double now;
#ifdef WIN32
	struct __timeb64 tstruct;
#else
	struct timeval stv;
#endif

	if (!trace->event.packet) {
		trace->event.packet = trace_create_packet();
	}

	if (!trace->event.waiting) {
		/* There is no packet event waiting for us, so create a new
		 * libtrace packet in the event structure and read the next
		 * packet into that.
		 *
		 * If a SLEEP event is reported this time around, the read
		 * packet can therefore be saved until the next time this
		 * function is called. */

		trace->event.psize=
			trace_read_packet(trace,trace->event.packet);
		if (trace->event.psize<1) {
			/* Return here, the test for event.size will sort out 
			 * the error  */
			if (trace_is_err(trace)) {
				trace_perror(trace, "read packet");
			}
			event.type = TRACE_EVENT_TERMINATE;
			trace_destroy_packet(trace->event.packet);
			trace->event.packet = NULL;
			packet->buffer = NULL;
			packet->header = NULL;
			packet->payload = NULL;
			packet->buf_control = TRACE_CTRL_EXTERNAL;
			return event;
		}
	}

	/* The goal here is to replicate the inter-packet gaps that are
	 * present in the trace. */

	ts=trace_get_seconds(trace->event.packet);

	/* Get the current walltime */
#ifdef WIN32
	_ftime64(&tstruct);
	now = tstruct.time + 
		((double)tstruct.millitm / 1000.0);
#else
	gettimeofday(&stv, NULL);
	now = stv.tv_sec + 
		((double)stv.tv_usec / 1000000.0);
#endif

	
	if (fabs(trace->event.tdelta)>1e-9) {
		/* Subtract the tdelta from the walltime to get a suitable
		 * "relative" time */
		now -= trace->event.tdelta; 

		/* If the trace timestamp is still in the future, return a 
		 * SLEEP event, otherwise return the packet */
		if (ts > now) {
			event.seconds = ts - 
				trace->event.trace_last_ts;
			trace->event.trace_last_ts = ts;
			event.type = TRACE_EVENT_SLEEP;
			trace->event.waiting = true;
			return event;
		}
	} else {
		/* Work out the difference between the walltime at the start 
		 * of the trace replay and the timestamp of the first packet 
		 * in the trace. This will be used to convert the walltime
		 * into a timeline that is relative to the timestamps in the
		 * trace file.
		 */
		trace->event.tdelta = now - ts;
	}

	/* The packet that we had read earlier is now ready to be returned
	 * to the user - switch all the pointers etc. over */	
	packet->type = trace->event.packet->type;
	packet->trace = trace->event.packet->trace;
	packet->header = trace->event.packet->header;
	packet->payload = trace->event.packet->payload;
	
	packet->buffer = trace->event.packet->buffer;
	packet->buf_control = trace->event.packet->buf_control;

	event.type = TRACE_EVENT_PACKET;

	trace->event.trace_last_ts = ts;
	trace->event.waiting = false;

	return event;
}