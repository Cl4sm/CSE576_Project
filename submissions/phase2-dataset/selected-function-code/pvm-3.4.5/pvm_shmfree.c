pvm_shmfree ( sseg )
struct segdesc *sseg;
{

struct shmid_ds shminfo;

	/* We haven't checked to see if this segment is locked... */
	/* We also haven't checked to see if we are attached/own it etc */

	/* Ok..  just grab it */
	(*sseg -> pagelock)(sseg -> lock);

	/* find out how many are attached to this segment */
	shmctl ( sseg -> segid, IPC_STAT, &shminfo);

	if ( shminfo.shm_nattch == 0 )
		shmctl( sseg -> segid, IPC_RMID, &shminfo);
	else /* what? */
		shmctl( sseg -> segid, IPC_RMID, &shminfo);
		/* yep, wrong... should send message to all so that they can detach */
		/* in a nice way */

	(*sseg -> pageunlock)( sseg -> lock );

 	/* reset structure variables */
	sseg -> bound = 0;
	sseg -> segControl = (struct msgctrl *) 0;
	sseg -> segid = 0;
	sseg -> len = 0;
	sseg -> start = (char *) NULL;
	sseg -> iAmOwner= 0;

	return 0;
}
