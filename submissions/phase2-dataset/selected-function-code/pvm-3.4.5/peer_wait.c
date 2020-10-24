int
peer_wait()
{
	struct msgboxhdr *inbp = (struct msgboxhdr *)pvminbox;

#ifndef USERECVSEMAPHORE

	PAGELOCK(&inbp->mb_lock);
	while (inbp->mb_read == inbp->mb_last)
#if defined(IMA_SUNMP) && defined(PVMUSEMUTEX)
		cond_wait(&inbp->mb_cond, &inbp->mb_lock);
#endif
#if defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP)
		pthread_cond_wait(&inbp->mb_cond, &inbp->mb_lock);
#endif
	PAGEUNLOCK(&inbp->mb_lock);

#else	/*USERECVSEMAPHORE*/

	struct sembuf sops;

	sops.sem_num = 0;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	while (inbp->mb_read == inbp->mb_last) {
		if (pvmdebmask & PDMSEM) {
			sprintf(pvmtxt, "peer_wait(): Waiting on semop id = %d\n", mysemid);
			pvmlogerror(pvmtxt);
		}
		if (semop(mysemid, &sops, 1) == -1) {
			if (errno != EINTR) {
				sprintf(pvmtxt, "peer_wait(): Error waiting for semop id = %d",
						mysemid);
				pvmlogperror(pvmtxt);
				return -1;
			}
		}
		else if (pvmdebmask & PDMSEM) {
			sprintf(pvmtxt, "peer_wait(): Processing Event on semop id = %d\n",
					mysemid);
			pvmlogerror(pvmtxt);
		}
	}

#endif	/*USERECVSEMAPHORE*/
	return 0;
}
