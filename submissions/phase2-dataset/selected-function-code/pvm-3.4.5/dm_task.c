dm_task(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct task *tp;
	struct pmsg *mp2;
	int where;

	hp = hp;

	if (upkuint(mp, &where)) {
		pvmlogerror("dm_task() bad msg format\n");
		return 0;
	}

#ifdef SHMEM
	mpp_setstatus(0);	/* get status from global info table */
#endif

	/* pack list of local tasks and reply to waiter */

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_TASKACK;
	mp2->m_wid = mp->m_wid;
	if (where & tidlmask) {
		if (tp = task_find(where)) {
			pkint(mp2, tp->t_tid);
			pkint(mp2, tp->t_ptid);
			pkint(mp2, myhostpart);
			pkint(mp2, tp->t_flag);
			pkstr(mp2, tp->t_a_out ? tp->t_a_out : "");
			pkint(mp2, tp->t_pid);
		}

	} else {
		for (tp = locltasks->t_link; tp != locltasks; tp = tp->t_link) {
			pkint(mp2, tp->t_tid);
			pkint(mp2, tp->t_ptid);
			pkint(mp2, myhostpart);
			pkint(mp2, tp->t_flag);
			pkstr(mp2, (tp->t_a_out ? tp->t_a_out : ""));
			pkint(mp2, tp->t_pid);
		}
	}
	sendmessage(mp2);
	return 0;
}
