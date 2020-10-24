dm_delhost(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int count;
	char *buf;
	struct pmsg *mp2;		/* DELHOSTACK message */
	struct pmsg *mp3;		/* HTDEL message */
	struct htab *ht_del;	/* hosts to delete */
	struct htab *ht_save;	/* remaining hosts */
	int hh;

/* XXX danger, this doesn't check if already doing a host add/delete */

	/* sanity check count */

	if (upkint(mp, &count)) {
		pvmlogerror("dm_delhost() bad msg format\n");
		return 0;
	}
	if (count < 1 || count > (tidhmask >> (ffs(tidhmask) - 1))) {
		pvmlogerror("dm_delhost() bad count\n");
		return 0;
	}

	/*
	* read host names from message, generate delete and save sets
	* and a DELHOSTACK reply message with result code for each host.
	* set SHUTDOWN flag for each host in delete set.
	*/

	ht_del = ht_new(1);
	ht_save = ht_new(1);
	ht_merge(ht_save, hosts);

	mp2 = mesg_new(0);
	mp2->m_tag = DM_DELHOSTACK;
	mp2->m_wid = mp->m_wid;
	mp2->m_dst = mp->m_src;

	mp3 = mesg_new(0);
	mp3->m_tag = DM_HTDEL;
	pkint(mp3, hosts->ht_serial);

	pkint(mp2, count);
	while (count-- > 0) {
		upkstralloc(mp, &buf);
		if (hp = nametohost(hosts, buf)) {
			if (tidtohost(ht_del, hp->hd_hostpart)) {
				pkint(mp2, PvmDupHost);

			} else {
				if (hp->hd_hostpart == myhostpart)
					pkint(mp2, PvmBadParam);

				else {
					ht_insert(ht_del, hp);
					ht_delete(ht_save, hp);
					pkint(mp3, hp->hd_hostpart);
					fin_to_host(hp);
					pkint(mp2, 0);
				}
			}

		} else
			pkint(mp2, PvmNoHost);
		PVM_FREE(buf);
	}

	/*
	* send HTDEL message to all hosts in ht_save set except us
	*/

	for (hh = ht_save->ht_last; hh > 0; hh--)
		if (hh != hosts->ht_local && (hp = ht_save->ht_hosts[hh])) {
			mp3->m_ref++;
			mp3->m_dst = hp->hd_hostpart | TIDPVMD;
			sendmessage(mp3);
		}
	pmsg_unref(mp3);

	/* reply to host that requested DELHOST operation */

	sendmessage(mp2);

	ht_free(ht_del);
	ht_free(ht_save);
	return 0;
}
