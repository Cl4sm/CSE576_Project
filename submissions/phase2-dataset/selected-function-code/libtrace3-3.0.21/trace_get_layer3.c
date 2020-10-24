DLLEXPORT void *trace_get_layer3(const libtrace_packet_t *packet,
		uint16_t *ethertype,
		uint32_t *remaining)
{
	void *iphdr;
	uint16_t dummy_ethertype;
	void *link;
	uint32_t dummy_remaining;
	libtrace_linktype_t linktype;

	if (!ethertype) ethertype=&dummy_ethertype;

	if (!remaining) remaining=&dummy_remaining;

	/* use l3 cache */
	if (packet->l3_header)
	{
		/*
		link = trace_get_packet_buffer(packet,&linktype,remaining);

		if (!link)
			return NULL;
		*/

		*ethertype = packet->l3_ethertype;
		/* *remaining -= (packet->l3_header - link); */
		*remaining = packet->l3_remaining;

		return packet->l3_header;
	}

	link = trace_get_layer2(packet,&linktype,remaining);
	iphdr = trace_get_payload_from_layer2(
			link,
			linktype,
			ethertype,
			remaining);

	for(;;) {
		if (!iphdr || *remaining == 0)
			break;
		switch(*ethertype) {
		case TRACE_ETHERTYPE_8021Q: /* VLAN */
			iphdr=trace_get_payload_from_vlan(
					  iphdr,ethertype,remaining);
			continue;
		case TRACE_ETHERTYPE_MPLS: /* MPLS */
			iphdr=trace_get_payload_from_mpls(
					  iphdr,ethertype,remaining);

			if (iphdr && ethertype == 0x0) {
				iphdr=trace_get_payload_from_ethernet(
						iphdr,ethertype,remaining);
			}
			continue;
		case TRACE_ETHERTYPE_PPP_SES: /* PPPoE */
			iphdr = trace_get_payload_from_pppoe(iphdr, ethertype,
					remaining);
			continue;
		default:
			break;
		}

		break;
	}

	if (!iphdr || *remaining == 0)
		return NULL;

	/* Store values in the cache for later */
	/* Cast away constness, nasty, but this is just a cache */
	((libtrace_packet_t*)packet)->l3_ethertype = *ethertype;
	((libtrace_packet_t*)packet)->l3_header = iphdr;
	((libtrace_packet_t*)packet)->l3_remaining = *remaining;

	return iphdr;
}