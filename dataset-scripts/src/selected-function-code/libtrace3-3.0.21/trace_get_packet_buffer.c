DLLEXPORT void *trace_get_packet_buffer(const libtrace_packet_t *packet,
		libtrace_linktype_t *linktype, uint32_t *remaining) {
	int cap_len;
	int wire_len;

	assert(packet != NULL);
	if (linktype) *linktype = trace_get_link_type(packet);
	if (remaining) {
		/* I think we should choose the minimum of the capture and
		 * wire lengths to be the "remaining" value. If the packet has
		 * been padded to increase the capture length, we don't want
		 * to allow subsequent protocol decoders to consider the 
		 * padding as part of the packet.
		 *
		 * For example, in Auck 4 there is a trace where the IP header
		 * length is incorrect (24 bytes) followed by a 20 byte TCP
		 * header. Total IP length is 40 bytes. As a result, the
		 * legacyatm padding gets treated as the "missing" bytes of
		 * the TCP header, which isn't the greatest. We're probably
		 * better off returning an incomplete TCP header in that case.
		 */
		
		cap_len = trace_get_capture_length(packet);
		wire_len = trace_get_wire_length(packet);

		assert(cap_len >= 0);

		/* There is the odd corrupt packet, e.g. in IPLS II, that have
		 * massively negative wire lens. We could assert fail here on
		 * them, but we could at least try the capture length instead.
		 * 
		 * You may still run into problems if you try to write that
		 * packet, but at least reading should work OK.
		 */
		if (wire_len < 0)
			*remaining = cap_len;
		else if (wire_len < cap_len)
			*remaining = wire_len;
		else
			*remaining = cap_len;
		/* *remaining = trace_get_capture_length(packet); */
	}
	return (void *) packet->payload;
}