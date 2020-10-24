dm_taskack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;
	struct pmsg *mp2;
	int i;
	char *p;

	if (!(wp = wait_get(hp, mp, WT_TASK)))
		return 0;

	/* append data to waiting message */

	mp2 = wp->wa_mesg;
	while (!upkint(mp, &i)) {
		pkint(mp2, i);			/* tid */
		upkint(mp, &i);			/* ptid */
		pkint(mp2, i);
		upkint(mp, &i);			/* host */
		pkint(mp2, i);
		upkint(mp, &i);			/* flag */
		pkint(mp2, i);
		upkstralloc(mp, &p);	/* a.out name */
		pkstr(mp2, p);
		PVM_FREE(p);
		upkint(mp, &i);			/* pid */
		pkint(mp2, i);
	}

	/* send message if we're the last waiter */

	if (wp->wa_peer == wp) {
		mp2->m_ref++;
		sendmessage(mp2);

	}
	wait_delete(wp);
	return 0;
}
