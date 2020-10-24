DLLEXPORT int trace_read_packet(libtrace_t *libtrace, libtrace_packet_t *packet) {

	assert(libtrace && "You called trace_read_packet() with a NULL libtrace parameter!\n");
	if (trace_is_err(libtrace))
		return -1;
	if (!libtrace->started) {
		trace_set_err(libtrace,TRACE_ERR_BAD_STATE,"You must call libtrace_start() before trace_read_packet()\n");
		return -1;
	}
	if (!(packet->buf_control==TRACE_CTRL_PACKET || packet->buf_control==TRACE_CTRL_EXTERNAL)) {
		trace_set_err(libtrace,TRACE_ERR_BAD_STATE,"Packet passed to trace_read_packet() is invalid\n");
		return -1;
	}
	assert(packet);
      
	/* Store the trace we are reading from into the packet opaque 
	 * structure */
	packet->trace = libtrace;

	/* Finalise the packet, freeing any resources the format module
	 * may have allocated it
	 */
	if (libtrace->format->fin_packet) {
		libtrace->format->fin_packet(packet);
	}


	if (libtrace->format->read_packet) {
		do {
			size_t ret;
                        int filtret;
			/* Clear the packet cache */
			trace_clear_cache(packet);
			ret=libtrace->format->read_packet(libtrace,packet);
			if (ret==(size_t)-1 || ret==0) {
				return ret;
			}
			if (libtrace->filter) {
				/* If the filter doesn't match, read another
				 * packet
				 */
                                filtret = trace_apply_filter(libtrace->filter, packet);
                                if (filtret == -1) {
                                        /* Error compiling filter, probably */
                                        return ~0U;
                                }
                                
                                if (filtret == 0) {
					++libtrace->filtered_packets;
					continue;
				}
			}
			if (libtrace->snaplen>0) {
				/* Snap the packet */
				trace_set_capture_length(packet,
						libtrace->snaplen);
			}
			++libtrace->accepted_packets;
			return ret;
		} while(1);
	}
	trace_set_err(libtrace,TRACE_ERR_UNSUPPORTED,"This format does not support reading packets\n");
	return ~0U;
}