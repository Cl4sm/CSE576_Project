dm_hostsyncack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;
	int i;

	if (!(wp = wait_get(hp, mp, WT_HOSTSYNC)))
		return 0;

	pkint(wp->wa_mesg, 0);
	upkuint(mp, &i);
	pkint(wp->wa_mesg, i);
	upkuint(mp, &i);
	pkint(wp->wa_mesg, i);
	sendmessage(wp->wa_mesg);
	wp->wa_mesg = 0;
	wait_delete(wp);
	return 0;
}
