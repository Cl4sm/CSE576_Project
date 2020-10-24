static struct einfo *
elist_alloc(register u_int32_t a, register u_char *e, register time_t t,
    register char *h, char *interface)
{
	register struct einfo *ep;
	register u_int size;
	static struct einfo *elist = NULL;
	static int eleft = 0;

	if (eleft <= 0) {
		/* Allocate some more */
		eleft = 16;
		size = eleft * sizeof(struct einfo);
		elist = (struct einfo *)malloc(size);
		if (elist == NULL) {
			syslog(LOG_ERR, "elist_alloc(): malloc: %m");
			exit(1);
		}
		MEMSET(elist, 0, size);
	}

	ep = elist++;
	--eleft;
	BCOPY(e, ep->e, 6);
	if (h == NULL && !initializing)
		h = getsname(a);
	if (h != NULL && !isdigit((int)*h)) {
		strncpy(ep->h, h, 34);
                ep->h[33] = '\0';
        }
	ep->t = t;
	if (interface != NULL) {
		strncpy(ep->i, interface, 16);
                ep->i[15] = '\0';
        }
	return (ep);
}