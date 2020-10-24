static int
hostexectasker(file, tid)
	char *file;
	int tid;
{
	int tids = 0;			/* tid from hosterforkexec */
	
	struct pmsg *rmp;
	struct task *tp = 0;
	int err = 0;			/* error code from forkexec */

	rmp = mesg_new(0);
	rmp->m_dst = pvmmytid;
	rmp->m_src = pvmmytid;

	rmp->m_tag = DM_EXECACK;

	if (err = hosterforkexec(tid,file, &tp)) {
		tids = err;
	} else {
			tp->t_ptid = 0;
			tp->t_outtid = 0;
			tp->t_trctid = 0;
			tp->t_sched = 0;
			tids = tp->t_tid;
	}

	pkint(rmp, 1);
	pkint(rmp, tids);
	sendmessage(rmp);
	
	return err;
}
