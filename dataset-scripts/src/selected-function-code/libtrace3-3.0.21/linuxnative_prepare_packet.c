static int linuxnative_prepare_packet(libtrace_t *libtrace UNUSED, 
		libtrace_packet_t *packet, void *buffer, 
		libtrace_rt_types_t rt_type, uint32_t flags) {

        if (packet->buffer != buffer &&
                        packet->buf_control == TRACE_CTRL_PACKET) {
                free(packet->buffer);
        }

        if ((flags & TRACE_PREP_OWN_BUFFER) == TRACE_PREP_OWN_BUFFER) {
                packet->buf_control = TRACE_CTRL_PACKET;
        } else
                packet->buf_control = TRACE_CTRL_EXTERNAL;


        packet->buffer = buffer;
        packet->header = buffer;
	packet->payload = (char *)buffer + 
		sizeof(struct libtrace_linuxnative_header);
	packet->type = rt_type;

	/*
	if (libtrace->format_data == NULL) {
		if (linuxnative_init_input(libtrace))
			return -1;
	}
	*/
	return 0;
	
}