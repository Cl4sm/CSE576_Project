get_ai(pai, afd, addr)
	const struct addrinfo *pai;
	const struct afd *afd;
	const char *addr;
{
	char *p;
	struct addrinfo *ai;

	ai = (struct addrinfo *)malloc(sizeof(struct addrinfo)
		+ (afd->a_socklen));
	if (ai == NULL)
		return NULL;

	memcpy(ai, pai, sizeof(struct addrinfo));
	ai->ai_addr = (struct sockaddr *)(void *)(ai + 1);
	memset(ai->ai_addr, 0, (size_t)afd->a_socklen);
#if HAVE_SOCKADDR_SA_LEN
	ai->ai_addr->sa_len = afd->a_socklen;
#endif
	ai->ai_addrlen = afd->a_socklen;
	ai->ai_addr->sa_family = ai->ai_family = afd->a_af;
	p = (char *)(void *)(ai->ai_addr);
	memcpy(p + afd->a_off, addr, (size_t)afd->a_addrlen);
	return ai;
}
