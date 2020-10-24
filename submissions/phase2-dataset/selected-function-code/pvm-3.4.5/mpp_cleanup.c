void
mpp_cleanup()
{
	struct peer *pp;
	struct shmid_ds shmds;

/*
	if (pvminbox && shmdt(pvminbox) == -1)
		pvmlogperror("mpp_cleanup() shmdt inbox");
*/

	if (myshmbufid != -1
	&& shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0) == -1)
		pvmlogperror("mpp_cleanup() shmctl IPC_RMID mybuf");

	if (peers) {
		for (pp = peers->p_link; pp != peers; pp = pp->p_link) {
			if (pp->p_buf) {
				shmdt(pp->p_buf);
				pp->p_buf = 0;
			}

			if (pp->p_shmid == -1 && pp->p_key)
				pp->p_shmid = shmget((key_t)pp->p_key, shmbufsiz, 0);
			if (pp->p_shmid != -1 &&
			shmctl(pp->p_shmid, IPC_RMID, (struct shmid_ds *)0) == -1) {
				sprintf(pvmtxt, "shmctl id=0x%x", pp->p_shmid);
				pvmlogperror(pvmtxt);
			}

#ifdef	USERECVSEMAPHORE
			if (pp->p_semid == -1 && pp->p_key)
				pp->p_semid = semget((key_t)pp->p_key, 1, PERMS);
			if (pp->p_semid != -1 && semctl(pp->p_semid, 0, IPC_RMID) == -1) {
				sprintf(pvmtxt, "semctl id=0x%x", pp->p_semid);
				pvmlogperror(pvmtxt);
			}
#endif
			pp->p_key = 0;
		}
	}
}
