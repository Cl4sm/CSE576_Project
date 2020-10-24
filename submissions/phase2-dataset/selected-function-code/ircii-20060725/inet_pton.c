inet_pton(af, src, dst)
	int af;
	const char *src;
	void *dst;
{

	switch (af) {
	case AF_INET:
		return (inet_pton4(src, dst, 1));
#ifdef INET6
	case AF_INET6:
		return (inet_pton6(src, dst));
#endif
	default:
		errno = EAFNOSUPPORT;
		return (-1);
	}
	/* NOTREACHED */
}
