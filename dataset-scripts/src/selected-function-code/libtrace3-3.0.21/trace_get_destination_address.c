DLLEXPORT struct sockaddr *trace_get_destination_address(
		const libtrace_packet_t *packet, struct sockaddr *addr)
{
	uint16_t ethertype;
	uint32_t remaining;
	void *l3;
	struct ports_t *ports;
	static struct sockaddr_storage dummy;

	if (!addr)
		addr=(struct sockaddr*)&dummy;

	l3 = trace_get_layer3(packet,&ethertype,&remaining);

	if (!l3)
		return get_destination_l2_address(packet,addr);

	switch (ethertype) {
		case TRACE_ETHERTYPE_IP: /* IPv4 */
		{
			struct sockaddr_in *addr4=(struct sockaddr_in*)addr;
			libtrace_ip_t *ip = (libtrace_ip_t*)l3;
			ports = (struct ports_t*)
				trace_get_payload_from_ip(ip,NULL,&remaining);
			addr4->sin_family=AF_INET;
			if (ports && remaining>=sizeof(*ports))
				addr4->sin_port=ports->dst;
			else
				addr4->sin_port=0;
			addr4->sin_addr=ip->ip_dst;
			return addr;
		}
		case TRACE_ETHERTYPE_IPV6: /* IPv6 */
		{
			struct sockaddr_in6 *addr6=(struct sockaddr_in6*)addr;
			libtrace_ip6_t *ip6 = (libtrace_ip6_t*)l3;
			ports = (struct ports_t*)
				trace_get_payload_from_ip6(ip6,NULL,&remaining);
			addr6->sin6_family=AF_INET6;
			if (ports && remaining>=sizeof(*ports))
				addr6->sin6_port=ports->dst;
			else
				addr6->sin6_port=0;
			addr6->sin6_flowinfo=0;
			addr6->sin6_addr=ip6->ip_dst;
			return addr;
		}
		default:
			return get_destination_l2_address(packet, addr);
	}
}