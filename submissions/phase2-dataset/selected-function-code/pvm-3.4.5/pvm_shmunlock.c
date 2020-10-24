int
pvm_shmunlock(lock)
PVM_LOCK lock;
{
struct sembuf lockop;

	lockop.sem_num = 0;
	lockop.sem_op = -1;
	lockop. sem_flg= 0;

	if (pvmdebmask & PDMSEM )
		pvmlogprintf ("Releasing lock %d \n", lock);

	while (semop(lock, &lockop, 1) == -1)
	{
		if (errno != EINTR )
		{
			pvmlogprintf(" Fatal Error releasing lock %d (errno %d)\n",
					lock, errno);
			return PvmSysErr;
		}
	}

	if (pvmdebmask & PDMSEM)
		pvmlogprintf("lock %d released \n", lock);

	return 0;
}
