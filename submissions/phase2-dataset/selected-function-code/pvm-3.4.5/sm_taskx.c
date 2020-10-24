int
sm_taskx(mp)
	struct pmsg *mp;
{
	int tid;
	struct task *tp;
	int i;

	if (mp->m_src != taskertid) {
		pvmlogprintf("sm_taskx() from t%x (not the tasker)\n", mp->m_src);
		return 0;
	}
	upkuint(mp, &tid);
	if (tp = task_find(tid)) {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("sm_taskx() t%x\n", tid);
		}
		upkuint(mp, &tp->t_status);
		upkuint(mp, &i);
		tp->t_utime.tv_sec = i;
		upkuint(mp, &i);
		tp->t_utime.tv_usec = i;
		upkuint(mp, &i);
		tp->t_stime.tv_sec = i;
		upkuint(mp, &i);
		tp->t_stime.tv_usec = i;
		task_cleanup(tp);
		task_free(tp);

	} else {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("sm_taskx() t%x (no such)\n", tid);
		}
	}
	return 0;
}
