				  sendip_data *pack) {
	ipv6_header *ipv6 = (ipv6_header *)pack->data;

	if(!(pack->modified&IPV6_MOD_VERSION)) {
		ipv6->ip6_vfc &= 0x0F;
		ipv6->ip6_vfc |= (6 << 4);
	}
	if(!(pack->modified&IPV6_MOD_PLEN)) {
		ipv6->ip6_plen = htons(data->alloc_len);
	}
	if(!(pack->modified&IPV6_MOD_NXT)) {
		ipv6->ip6_nxt = (u_int8_t)IPPROTO_NONE;
	}
	if(!(pack->modified&IPV6_MOD_HLIM)) {
		ipv6->ip6_hlim = 32;
	}

	return TRUE;
}
