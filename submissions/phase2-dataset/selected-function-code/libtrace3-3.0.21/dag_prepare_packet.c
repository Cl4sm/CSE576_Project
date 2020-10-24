static int dag_prepare_packet(libtrace_t *libtrace, libtrace_packet_t *packet,
		void *buffer, libtrace_rt_types_t rt_type, uint32_t flags) {

        dag_record_t *erfptr;
	/* If the packet previously owned a buffer that is not the buffer
         * that contains the new packet data, we're going to need to free the
         * old one to avoid memory leaks */
        if (packet->buffer != buffer &&
                        packet->buf_control == TRACE_CTRL_PACKET) {
                free(packet->buffer);
        }

	/* Set the buffer owner appropriately */
	if ((flags & TRACE_PREP_OWN_BUFFER) == TRACE_PREP_OWN_BUFFER) {
		packet->buf_control = TRACE_CTRL_PACKET;
	} else 
		packet->buf_control = TRACE_CTRL_EXTERNAL;
	
	/* Update packet pointers and type appropriately */
        erfptr = (dag_record_t *)buffer;
        packet->buffer = erfptr;
        packet->header = erfptr;
        packet->type = rt_type;

        if (erfptr->flags.rxerror == 1) {
                /* rxerror means the payload is corrupt - drop the payload
                 * by tweaking rlen */
                packet->payload = NULL;
                erfptr->rlen = htons(erf_get_framing_length(packet));
        } else {
                packet->payload = (char*)packet->buffer
                        + erf_get_framing_length(packet);
        }

        if (libtrace->format_data == NULL) {
                dag_init_format_data(libtrace);
        }

	/* Update the dropped packets counter, using the value of the ERF
	 * loss counter */
        DATA(libtrace)->drops += ntohs(erfptr->lctr);

        return 0;

}