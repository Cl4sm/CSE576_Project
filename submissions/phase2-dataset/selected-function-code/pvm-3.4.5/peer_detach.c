peer_detach(pp)
	struct peer *pp;
{
	struct shmid_ds shmds;
	char *p;

	if (pp->p_tid != pvmmytid) {
	/*
	* zero p_buf before destroying it in case we're interrupted right here.
	* yes, it happend.
	*/
		if (p = pp->p_buf) {
			pp->p_buf = 0;
			if (shmdt(p) == -1)
				pvmlogperror("peer_detach() shmdt");
		}

		if (!shmctl(pp->p_shmid, IPC_STAT, &shmds) && shmds.shm_nattch == 0) {
			shmctl(pp->p_shmid, IPC_RMID, (struct shmid_ds *)0);

#ifdef	USERECVSEMAPHORE
			if (pp->p_semid == -1 && pp->p_key)
				pp->p_semid = semget((key_t)pp->p_key, 1, PERMS);
			if (pp->p_semid != -1)
				semctl(pp->p_semid, 0, IPC_RMID);
#endif
		}
	}

	LISTDELETE(pp, p_link, p_rlink);
	PVM_FREE(pp);
	return 0;
}
