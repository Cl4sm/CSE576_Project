print_fdset(pad, n, f)
	char *pad;			/* label at head */
	int n;				/* max fd + 1 */
#ifdef FDSETNOTSTRUCT
	fd_set *f;			/* fd set */
#else
	struct fd_set *f;	/* fd set */
#endif
{
	int i;
	char *s = "";

	pvmlogprintf("%s", pad);
	for (i = 0; i < n; i++)
		if (FD_ISSET(i, f)) {
			pvmlogprintf("%s%d", s, i);
			s = ",";
		}
	pvmlogprintf("\n");
	return 0;
}
