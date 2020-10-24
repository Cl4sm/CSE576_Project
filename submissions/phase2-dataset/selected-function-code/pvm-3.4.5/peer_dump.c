peer_dump()
{
	struct peer *pp;
	struct shmid_ds shmds;

	pvmlogerror("peer_dump()\n");
	for (pp = peers->p_link; pp != peers; pp = pp->p_link) {
		if (shmctl(pp->p_shmid, IPC_STAT, &shmds) == -1) {
			sprintf(pvmtxt, "peer_dump() shmctl STAT %d", pp->p_shmid);
			pvmlogperror(pvmtxt);
			sprintf(pvmtxt,
					" t%x key=0x%x shmid=%d semid=%d shmlen=%d\n",
					pp->p_tid, pp->p_key, pp->p_shmid, pp->p_semid, pp->p_dlen);
			pvmlogerror(pvmtxt);

		} else {
			sprintf(pvmtxt,
					" t%x key=0x%x shmid=%d semid=%d shmlen=%d shmna=%d\n",
					pp->p_tid, pp->p_key, pp->p_shmid, pp->p_semid, pp->p_dlen,
					(int)shmds.shm_nattch);
			pvmlogerror(pvmtxt);
		}
	}
	return 0;
}
