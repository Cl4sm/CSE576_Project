tm_tickle(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int nar;
	int arg[10];
	int i;
	char *p;
	struct hostd *hp;

	upkint(mp, &nar);
	if (nar < 1 || nar > 10) {
		pvmlogprintf("tm_tickle() bad msg format\n");
		return 0;
	}
	for (i = 0; i < nar; i++)
		upkint(mp, &arg[i]);
	while (i < sizeof(arg)/sizeof(arg[0]))
		arg[i++] = 0;

	pvmlogprintf("tm_tickle() #");
	for (i = 0; i < nar; i++)
		pvmlogprintf(" %d", arg[i]);
	pvmlogprintf("\n");

	mp = replymessage(mp);

	switch (arg[0]) {

	case 0:
		i_dump(1);
		pkint(mp, 0);
		break;

	case 1:
		ht_dump(hosts);
		pkint(mp, 0);
		break;

	case 2:
		task_dump();
		pkint(mp, 0);
		break;

	case 3:
		wait_dumpall();
		pkint(mp, 0);
		break;

	case 4:
		mb_dumpall();
		pkint(mp, 0);
		break;

	case 5:
		pkint(mp, 1);
		pkint(mp, pvmdebmask);
		break;

	case 6:
		pvmdebmask = arg[1];
		pvmlogprintf("tm_tickle() debug mask is %x (%s)\n",
				pvmdebmask, debug_flags(pvmdebmask));
		pkint(mp, 0);
		break;

	case 7:
		if (arg[1] > 0 && arg[1] < 50) {
			nopax = arg[1];
			pvmlogprintf("tm_tickle() nopax is %d\n", nopax);
		} else
			pvmlogprintf("tm_tickle() bogus nopax %d\n", arg[1]);
		pkint(mp, 0);
		break;

	case 8:
		pkint(mp, 1);
		if ((hp = tidtohost(hosts, arg[1]))
		&& hp != hosts->ht_hosts[hosts->ht_local]) {
			pvmlogprintf("tm_tickle() failing %s\n", hp->hd_name);
			hostfailentry(hp);
			ht_delete(hosts, hp);
			if (newhosts)
				ht_delete(newhosts, hp);
			pkint(mp, 1);
		} else {
			if (hp)
				pvmlogprintf("tm_tickle() can't fail %s\n", hp->hd_name);
			else
				pvmlogprintf("tm_tickle() no such host %x\n", arg[1]);
			pkint(mp, 0);
		}
		break;

	case 9:
#ifdef	STATISTICS
		dump_statistics();
		if (arg[1])
			reset_statistics();
#else
		pvmlogerror("tm_tickle() statistics not compiled in\n");
#endif
		pkint(mp, 0);
		break;

	case 100:
#ifdef SHMEM
		peer_dump();
		pidtid_dump();
#else
		pvmlogerror("tm_tickle() not using shared memory\n");
#endif
		pkint(mp, 0);
		break;

	case 20:
		{
			struct pmsg *mp2;
			int i, j;

			for (j = 0; j < arg[1]; j++) {
				mp2 = mesg_new(0);
				for (i = 0; i < 1000; i++)
					pkint(mp2, i);
				mp2->m_dst = arg[2];
				mp2->m_tag = DM_NULL;
				sendmessage(mp2);
			}
			pkint(mp, 0);
		}
		break;

	default:
		pvmlogprintf("tm_tickle() don't know #%d\n", arg[0]);
		pkint(mp, 0);
		break;
	}

	sendmessage(mp);
	return 0;
}
