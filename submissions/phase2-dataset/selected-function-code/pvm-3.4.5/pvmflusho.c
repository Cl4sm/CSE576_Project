static int
pvmflusho()
{
	struct tobuf *op;

	if (outlogff) {
		if (tobuflist && tobuflist->o_link != tobuflist) {
			if (pvmshowtaskid)
				pvmlogerror("child task(s) still running.  waiting...\n");
			while (tobuflist->o_link != tobuflist) {

	/* XXX mroute() does return after ctrl messages received */

				if ( mroute(0, 0, 0, (struct timeval *)0) < 0 )
					break;
			}
		}
		pvm_catchout((FILE *)0);
	}

	if (tobuflist) {
		while (tobuflist->o_link != tobuflist)
			tobuf_free(tobuflist->o_link);
		PVM_FREE(tobuflist);
		tobuflist = 0;
	}
	return 0;
}
