static int erf_prepare_packet(libtrace_t *libtrace, libtrace_packet_t *packet,
		void *buffer, libtrace_rt_types_t rt_type, uint32_t flags) {
	
	dag_record_t *erfptr;
	
	if (packet->buffer != buffer && 
		packet->buf_control == TRACE_CTRL_PACKET) {
		free(packet->buffer);
	}

	if ((flags & TRACE_PREP_OWN_BUFFER) == TRACE_PREP_OWN_BUFFER) {
		packet->buf_control = TRACE_CTRL_PACKET;
	} else
	        packet->buf_control = TRACE_CTRL_EXTERNAL;
	
	
	packet->type = rt_type;
	packet->buffer = buffer;
	packet->header = buffer;
	erfptr = (dag_record_t *)packet->buffer;
	if (erfptr->flags.rxerror == 1) {
		packet->payload = NULL;
	} else {
		packet->payload = (char*)packet->buffer + erf_get_framing_length(packet);
	}
	
	if (libtrace->format_data == NULL) {
		/* Allocates the format_data structure */
		if (erf_init_input(libtrace)) 
			return -1;
	}

	/* Check for loss */
	if (erfptr->type == TYPE_DSM_COLOR_ETH) {
		/* No idea how we get this yet */

	} else if (erfptr->lctr) {
		DATA(libtrace)->drops += ntohs(erfptr->lctr);
	}

	return 0;
}