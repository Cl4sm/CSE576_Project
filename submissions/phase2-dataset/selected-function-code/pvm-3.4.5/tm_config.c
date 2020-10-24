tm_config(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int hh;
	struct hostd *hp;

	mp = replymessage(mp);
	pkint(mp, hosts->ht_cnt);
	pkint(mp, hosts->ht_narch);
	for (hh = 1; hh <= hosts->ht_last; hh++) {
		if (hp = hosts->ht_hosts[hh]) {
			pkint(mp, hp->hd_hostpart);
			pkstr(mp, hp->hd_name);
			pkstr(mp, hp->hd_arch ? hp->hd_arch : "");
			pkint(mp, hp->hd_speed);
			pkint(mp, hp->hd_dsig);
		}
	}
	sendmessage(mp);
	return 0;
}
