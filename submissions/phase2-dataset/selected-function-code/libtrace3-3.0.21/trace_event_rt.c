static libtrace_eventobj_t trace_event_rt(libtrace_t *trace,
					libtrace_packet_t *packet) 
{
	libtrace_eventobj_t event = {0,0,0.0,0};
	libtrace_err_t read_err;

	assert(trace);
	assert(packet);
	
	if (trace->format->get_fd) {
		event.fd = trace->format->get_fd(trace);
	} else {
		event.fd = 0;
	}

	do {

		event.size = rt_read_packet_versatile(trace, packet, 0);
		if (event.size == -1) {
			read_err = trace_get_err(trace);
			if (read_err.err_num == EAGAIN) {
				/* No data available - do an IOWAIT */
				event.type = TRACE_EVENT_IOWAIT;
			}
			else {
				trace_perror(trace, "Error doing a non-blocking read from rt");
				event.type = TRACE_EVENT_PACKET;
				break;
			}
		} else if (event.size == 0) {
			/* RT gives us a specific indicator that there will be 
			 * no more packets. */
			if (packet->type == TRACE_RT_END_DATA)
				event.type = TRACE_EVENT_TERMINATE;
			else {
				/* Since several RT messages can have zero-byte
				 * length (once the framing is removed), an 
				 * event size of zero can still indicate a 
				 * PACKET event */
				event.type = TRACE_EVENT_PACKET;
                                trace->accepted_packets ++;
                        }

		}	
		else {
			event.type = TRACE_EVENT_PACKET;
                        trace->accepted_packets ++;
		}

		if (trace->filter && event.type == TRACE_EVENT_PACKET) {
			if (!trace_apply_filter(trace->filter, packet)) {
				trace_clear_cache(packet);
				trace->filtered_packets ++;
                                continue;
			}
		}

		break;	
	} while (1);

	return event;
}