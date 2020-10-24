pvm_lockdt ( sseg )
struct segdesc *sseg;
{
int lockcmd;
int semid;
/* see above routine GEF */
/* union semun lockarg; */
union semun {
	int val;
	struct semid_ds *buf;
		ushort *array;
} lockarg;



	if (!sseg || !sseg -> lock )
		return 0;

	if ( sseg -> iAmOwner )      /* I own the lock, will free it */
	{

		/* Lock the page -- upon deletion, any tasks waiting on the */
	 	/* 	semaphore will be woken up and returned an error */
		(* sseg -> pagelock ) (sseg->lock);
		lockcmd = IPC_RMID;
		lockarg.val = 0;
		semctl ( sseg -> lock, 0, lockcmd, lockarg );
		/* we can now reset the ownership variable if the shm seg has gone */
		if (!sseg->start)
			sseg -> iAmOwner= 0;
	}

	sseg -> lock = 0;
	sseg -> pagelock = sseg -> pageunlock = 0;

	return 0;
}
