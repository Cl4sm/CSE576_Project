static void
alist_alloc(register struct ainfo *ap)
{
	register u_int size;

	if (ap->esize == 0) {
		syslog(LOG_ERR, "alist_alloc(): esize 0, can't happen");
		exit(1);
	}
	if (ap->ecount < ap->esize)
		return;
	ap->esize += 2;
	size = ap->esize * sizeof(ap->elist[0]);
	ap->elist = (struct einfo **)realloc(ap->elist, size);
	if (ap->elist == NULL) {
		syslog(LOG_ERR, "alist_alloc(): realloc(): %m");
		exit(1);
	}
	size = (ap->esize - ap->ecount) * sizeof(ap->elist[0]);
	MEMSET(&ap->elist[ap->ecount], 0, size);
}