int
dm_sendsig(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int tid;
	int signum;
	struct task *tp;

	hp = hp;

	if (upkuint(mp, &tid) || upkint(mp, &signum)) {
		pvmlogerror("dm_sendsig() bad msg format\n");
		return 0;
	}
	if (tp = task_find(tid)) {
		ppi_kill(tp, signum);

	} else
		if (pvmdebmask & (PDMTASK|PDMAPPL)) {
			pvmlogprintf("dm_sendsig() signal for t%x scrapped\n", tid);
		}
	return 0;
}
