peer_wake(pp)
	struct peer *pp;
{
	struct sembuf	sops;

	if (pp->p_semid == -1
	&& (pp->p_semid = semget((key_t)pp->p_key, 1, PERMS)) == -1) {
		sprintf(pvmtxt, "peer_wake() semget t%x key 0x%x\n",
				pp->p_tid, pp->p_key);
		pvmlogperror(pvmtxt);
		return;
	}
	sops.sem_num = 0;
	sops.sem_op = 1;
	sops.sem_flg = 0;
	if (semop(pp->p_semid, &sops, 1) == -1) {
		sprintf(pvmtxt, "peer_wake() semop t%x\n", pp->p_tid);
		pvmlogperror(pvmtxt);

	} else if (pvmdebmask & PDMSEM) {
		sprintf(pvmtxt,"peer_wake() sem %d\n", pp->p_semid);
		pvmlogerror(pvmtxt);
	}
}
