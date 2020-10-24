static struct sockaddr *get_source_ethernet_address(
	libtrace_ether_t *ethernet, struct sockaddr *addr)
{
	static struct sockaddr_storage dummy;
#ifdef HAVE_NETPACKET_PACKET_H
/* Use linux's sockaddr_ll structure */
	struct sockaddr_ll *l2addr;

	if (addr)
		l2addr = (struct sockaddr_ll*)addr;
	else
		l2addr = (struct sockaddr_ll*)&dummy;
	
	l2addr->sll_family = AF_PACKET;
	l2addr->sll_protocol = ethernet->ether_type;
	l2addr->sll_ifindex = 0; /* Irrelevant */
	l2addr->sll_hatype = ARPHRD_ETHER; 
	l2addr->sll_pkttype = PACKET_OTHERHOST;
	l2addr->sll_halen = 6;
	memcpy(l2addr->sll_addr,ethernet->ether_shost, 6);

	return (struct sockaddr*)l2addr;
#else
/* Use BSD's sockaddr_dl structure */
	struct sockaddr_dl *l2addr;

	if (addr)
		l2addr = (struct sockaddr_dl *)addr;
	else
		l2addr = (struct sockaddr_dl *)&dummy;
	
	l2addr->sdl_family = AF_LINK;
#if HAVE_SDL_LEN == 1
	l2addr->sdl_len = sizeof(struct sockaddr_dl);
#endif
	l2addr->sdl_index = 0; /* Unused */
	l2addr->sdl_alen = 6; /* Address length  */
	l2addr->sdl_nlen = 0; /* No name in here - this *should* work, right? */
	l2addr->sdl_slen = 0;	
	l2addr->sdl_type = 0; /* Hopefully zero is OK for this value too */
	memcpy(l2addr->sdl_data, ethernet->ether_shost, 6);

	return (struct sockaddr *)l2addr;
#endif
}