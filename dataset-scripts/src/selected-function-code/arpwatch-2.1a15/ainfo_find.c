static struct ainfo *
ainfo_find(register u_int32_t a)
{
	register u_int size;
	register struct ainfo *ap;

	ap = &ainfo_table[a & (HASHSIZE - 1)];
	for (;;) {
		if (ap->esize == 0) {
			/* Emtpy cell; use it */
			ap->a = a;
			break;
		}
		if (a == ap->a)
			break;

		if (ap->next != NULL) {
			/* Try linked cell */
			ap = ap->next;
			continue;
		}
		/* We collided, allocate new struct */
		ap->next = newainfo();
		ap = ap->next;
		ap->a = a;
		break;
	}
	if (ap->esize == 0) {
		ap->esize = 2;
		size = sizeof(ap->elist[0]) * ap->esize;
		ap->elist = (struct einfo **)malloc(size);
		if (ap->elist == NULL) {
			syslog(LOG_ERR, "ainfo_find(): malloc: %m");
			exit(1);
		}
		MEMSET(ap->elist, 0, size);
	}
	return (ap);
}