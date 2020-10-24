dm_htupd(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int count;			/* number of hosts in message */
	int hh;
	char buf[16];		/* for converting sockaddr */
	struct pmsg *mp2;

	/* unpack new host table params */

	newhosts = ht_new(1);
	newhosts->ht_local = hosts->ht_local;
	upkint(mp, &newhosts->ht_serial);
	upkint(mp, &newhosts->ht_master);
	upkint(mp, &newhosts->ht_cons);

	/* add current hosts to the table */

	ht_merge(newhosts, hosts);

	/* unpack new hosts and add to table */

	/* XXX if we find a host already in table we should kill it with
	   XXX hostfail and put the new one in its place */

	upkint(mp, &count);
	while (count-- > 0) {
		upkint(mp, &hh);
		hp = hd_new(hh);
		upkstralloc(mp, &hp->hd_name);
		upkstralloc(mp, &hp->hd_arch);
		upkstr(mp, buf, sizeof(buf));
		hex_inadport(buf, &hp->hd_sad);
		upkint(mp, &hp->hd_mtu);
		upkint(mp, &hp->hd_speed);
		upkint(mp, &hp->hd_dsig);
		ht_insert(newhosts, hp);
		hd_unref(hp);
	}

	if (pvmdebmask & PDMHOST) {
		pvmlogerror("dm_htupd() new host table:\n");
		ht_dump(newhosts);
	}
	runstate = PVMDHTUPD;

	/* reply to sender that we have new host table */

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_ctx = mp->m_ctx;
	mp2->m_tag = DM_HTUPDACK;
	mp2->m_wid = mp->m_wid;
	sendmessage(mp2);
	return 0;
}
