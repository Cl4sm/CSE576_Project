static char *sockaddr_to_string(struct sockaddr *addrptr, char *space,
		int spacelen) {

	assert(addrptr && space);
	assert(spacelen > 0);
	
	if (addrptr->sa_family == AF_INET) {
		struct sockaddr_in *v4 = (struct sockaddr_in *)addrptr;
		inet_ntop(AF_INET, &(v4->sin_addr), space, spacelen);
	}

	else if (addrptr->sa_family == AF_INET6) {
		struct sockaddr_in6 *v6 = (struct sockaddr_in6 *)addrptr;
		inet_ntop(AF_INET6, &(v6->sin6_addr), space, spacelen);
	}
#ifdef HAVE_NETPACKET_PACKET_H
	else if (addrptr->sa_family == AF_PACKET) {
		struct sockaddr_ll *l2addr = (struct sockaddr_ll *)addrptr;
		uint8_t *macbytes = (uint8_t *)l2addr->sll_addr;

		snprintf(space, spacelen, "%02x:%02x:%02x:%02x:%02x:%02x",
				macbytes[0], macbytes[1], macbytes[2],
				macbytes[3], macbytes[4], macbytes[5]);

	}
#else
	else if (addrptr->sa_family == AF_LINK) {
		struct sockaddr_dl *l2addr = (struct sockaddr_dl *)addrptr;
		uint8_t *macbytes = (uint8_t *)l2addr->sdl_data;

		snprintf(space, spacelen, "%02x:%02x:%02x:%02x:%02x:%02x",
				macbytes[0], macbytes[1], macbytes[2],
				macbytes[3], macbytes[4], macbytes[5]);
	
	}
#endif
	else {
		space[0] = '\0';
		return NULL;
	}

	return space;

}