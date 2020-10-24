int
tm_tasker(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int regme;
	struct pmsg *mp2;

	if (upkint(mp, &regme)) {
		pvmlogerror("tm_tasker() bad msg format\n");
		return 0;
	}
	mp2 = replymessage(mp);

	if (regme) {
		if (taskertid) {
			pkint(mp2, PvmAlready);

		} else {
			taskertid = tp->t_tid;
			tp->t_flag |= TF_ISTASKER;
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("tm_tasker() register t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);
		}

	} else {
		if (taskertid == tp->t_tid) {
			taskertid = 0;
			tp->t_flag &= ~TF_ISTASKER;
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("tm_tasker() unregister t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);

		} else {
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("tm_tasker() t%x tries to unregister?\n",
						tp->t_tid);
			}
			pkint(mp2, PvmNoTask);
		}
	}
	sendmessage(mp2);
	return 0;
}
