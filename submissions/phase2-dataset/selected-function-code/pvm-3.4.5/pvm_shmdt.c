pvm_shmdt ( sseg )
struct segdesc *sseg;
{

struct shmid_ds shminfo;

	if (!sseg ||  !sseg -> bound )       /* not bound to the shared segment */
	 	return (0);

	(*sseg -> pagelock)(sseg -> lock);

	sseg -> segControl -> nattach --;

	/* find out how many are attached to this segment */

	shmctl ( sseg -> segid, IPC_STAT, &shminfo);

	if ( shminfo.shm_nattch == 1 ) /* I am the only one left, destroy */
		shmctl( sseg -> segid, IPC_RMID, &shminfo);

	(*sseg -> pageunlock)( sseg -> lock );

	shmdt( sseg -> start); /* the segment will get destroyed after detach */

 	/* reset structure variables */
	sseg -> bound = 0;
	sseg -> segControl = (struct msgctrl *) 0;
	sseg -> segid = 0;
	sseg -> len = 0;
	sseg -> start = (char *) NULL;

	/* iAmOwner is shared between the shm seg and the semaphore lock */
	/* thus we leave this variable to be reset by the pvm_lockdt() call */
	/* unless the lock as already gone.. yes the lock we used above! */
	if (!sseg->lock)
		sseg -> iAmOwner= 0;

	return 0;
}
