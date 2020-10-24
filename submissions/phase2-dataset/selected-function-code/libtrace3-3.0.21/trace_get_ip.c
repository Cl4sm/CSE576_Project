libtrace_ip_t *trace_get_ip(libtrace_packet_t *packet) 
{
	uint16_t ethertype;
	void *ret;

	uint32_t remaining = trace_get_capture_length(packet);

	ret = trace_get_layer3(packet,&ethertype,&remaining);

	if (!ret || ethertype!=TRACE_ETHERTYPE_IP)
		return NULL;

	/* Make sure we have at least a base IPv4 header */
	if (remaining < sizeof(libtrace_ip_t)) 
		return NULL;
	
	/* Not an IPv4 packet */
	if (((libtrace_ip_t*)ret)->ip_v != 4)
		return NULL;

	return (libtrace_ip_t*)ret;
}