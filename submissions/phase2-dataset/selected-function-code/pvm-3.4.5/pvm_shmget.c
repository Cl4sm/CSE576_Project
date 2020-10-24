pvm_shmget(key, len, iAmOwner)
key_t key;
int len;
int iAmOwner;
{
struct segdesc *rseg;
struct shmid_ds shminfo;
int flags = 0;

	if ((rseg = (struct segdesc *) TALLOC(1, struct segdesc, "shmget")) == 0)
	{
		pvmlogerror( "pvm_shmget (): No memory for seg descriptor \n");
		return (struct segdesc *) NULL;
	}

	BZERO(rseg, sizeof(struct segdesc));

	 if (iAmOwner)
	 	flags =IPC_CREAT | IPC_EXCL | PERMS;
	 else
		flags = PERMS;

	if ((rseg -> segid = shmget(key, len, flags)) == -1
		 && pvmdebmask & PDMNODE)
	{
		pvmlogperror ("pvm_shmget (): could not create shared segment \n");
		/* GEF */
		/* there was just a free here... wonder if pvm_free will work */
/* 		FREE(rseg); */
		PVM_FREE(rseg);
		return (struct segdesc *) NULL;
	}

	shmctl(rseg -> segid , IPC_STAT, &shminfo);

	rseg -> len= shminfo.shm_segsz;
	rseg -> bound = 0;
	rseg -> segControl = (struct msgctrl *) NULL;
	rseg -> pagelock = rseg -> pageunlock = 0;
	rseg -> lock = 0;
	rseg -> iAmOwner= iAmOwner;

	return rseg;

}
