DLLEXPORT char *trace_get_destination_address_string(
		const libtrace_packet_t *packet, char *space, int spacelen) {

	struct sockaddr_storage addr;
	struct sockaddr *addrptr;
	
	static char staticspace[INET6_ADDRSTRLEN];

	if (space == NULL || spacelen == 0) {
		space = staticspace;
		spacelen = INET6_ADDRSTRLEN;
	}

	addrptr = trace_get_destination_address(packet, 
			(struct sockaddr *)&addr);

	if (addrptr == NULL)
		return NULL;
	
	return sockaddr_to_string(addrptr, space, spacelen);
}