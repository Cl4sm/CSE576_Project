int
dm_resetack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	hp = hp;

	if (!(wp = wait_get((struct hostd*)0, mp, 0)))
		return 0;

	if (wp->wa_tid && wp->wa_mesg) {
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;
	}

	mb_tidy_reset(wp->wa_on);

	wait_delete(wp);
	return 0;
}
