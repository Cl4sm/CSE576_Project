static int dpdk_prepare_packet(libtrace_t *libtrace UNUSED,
		libtrace_packet_t *packet, void *buffer,
		libtrace_rt_types_t rt_type, uint32_t flags) {
    assert(packet);
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

    /* Don't use pktmbuf_mtod will fail if the packet is a copy */
    packet->payload = (char *)buffer + dpdk_get_framing_length(packet);
    packet->type = rt_type;
    return 0;
}