struct waitc *
wait_new(kind)
	int kind;
{
	static int lastwid = 0;		/* last wid assigned */

	int startwid;				/* to detect when we've tried all */
	int wid;
	struct waitc *wp, *wp2;

	/*
	* find a unique wid by incrementing lastwid and stepping through
	* waitlist until we find a vacant slot.
	*/

	if (++lastwid > widrange)
		lastwid = 1;
	startwid = lastwid;
	wp = waitlist;

	while (1) {
		wid = widbase + lastwid;
		while (wp->wa_wid < wid)
			if ((wp = wp->wa_link) == waitlist)
				break;

		if (wp->wa_wid != wid)
			break;

		if (++lastwid > widrange) {
			lastwid = 1;
			wp = waitlist;
		}
		if (lastwid == startwid) {
			pvmlogprintf("wait_new() out of wids\n");
			pvmbailout(0);		/* XXX this is kinda harsh */
		}
	}

	/*
	* initialze new w context and add to list
	*/

	if (!(wp2 = TALLOC(1, struct waitc, "wait"))) {
		pvmlogprintf("wait_new() can't get memory\n");
		pvmbailout(0);
	}
	wp2->wa_wid = wid;
	wp2->wa_kind = kind;
	wp2->wa_peer = wp2->wa_rpeer = wp2;

	wp2->wa_on = wp2->wa_tid = wp2->wa_dep = 0;
	wp2->wa_mesg = 0;
	wp2->wa_count = 0;
	wp2->wa_spec = 0;

	LISTPUTBEFORE(wp, wp2, wa_link, wa_rlink);
	if (pvmdebmask & PDMWAITC) {
		pvmlogprintf("wait_new():\n");
		wait_dump(wp2);
	}
	return wp2;
}
