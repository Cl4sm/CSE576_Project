static int atmhdr_prepare_packet(libtrace_t *libtrace, 
		libtrace_packet_t *packet, void *buffer, 
		libtrace_rt_types_t rt_type, uint32_t flags) {

	/* If the packet previously owned a buffer that was not the buffer
	 * containing the new packet data, we need to free the old one to 
	 * avoid leaking memory */
	if (packet->buffer != buffer &&
                        packet->buf_control == TRACE_CTRL_PACKET) {
                free(packet->buffer);
        }

	/* Set the buffer owner appropriately */
        if ((flags & TRACE_PREP_OWN_BUFFER) == TRACE_PREP_OWN_BUFFER) {
                packet->buf_control = TRACE_CTRL_PACKET;
        } else
                packet->buf_control = TRACE_CTRL_EXTERNAL;

	/* Update the packet pointers appropriately */
	packet->buffer = buffer;
	packet->header = buffer;
	packet->payload = (void*)((char*)packet->buffer + 
			libtrace->format->get_framing_length(packet));

	/* Set the packet type */
	packet->type = rt_type;

	return 0;
}