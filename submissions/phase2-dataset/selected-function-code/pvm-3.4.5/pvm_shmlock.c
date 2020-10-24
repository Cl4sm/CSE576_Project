pvm_shmlock( lock )
PVM_LOCK lock;
{
struct sembuf lockop;

	lockop.sem_num= 0;
	lockop.sem_op= 1;
	lockop.sem_flg= 0;

	if (pvmdebmask & PDMSEM )
	 	pvmlogprintf ("Acquiring lock %d \n" , lock) ;

	while (semop(lock, &lockop, 1) == -1)
	{
		if (errno != EINTR)
		{
			pvmlogprintf(" Fatal Error acquiring lock %d (errno %d)\n",
					lock, errno);
			return PvmSysErr;
		}
		/* GEF */
		pvmsleep (1);	/* don't even ask... */
	}

	if (pvmdebmask & PDMSEM )
		 pvmlogprintf("lock %d acquired \n", lock);

	return 0;
}
