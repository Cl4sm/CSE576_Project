int
pvm_shmmsginit( key, localseg )
key_t *key;
struct segdesc **localseg;
{
key_t lockkey;
key_t shmkey;

int first;
int lock;
int nretries;
char *tmp_shmseg_ptr;

struct segdesc * myshmseg;

	/* Step 1 - create my local shared memory segment */
	/* Step 2 - create lock for local shared segment */
	/* Step 3 - create the local midlist for posted sends and receives */

	nretries = 0;

	/*  === Step 1: === */
	/* This section really just makes a segment descriptor and a shm key */

	first = shmkey = (key_t) pvm_shmkey(getpid());
	do
	{
		if ( (myshmseg = pvm_shmget(shmkey, SHMSEGLEN, SHM_OWNER)) !=
				(struct segdesc *) NULL )
		{
	        if ( (tmp_shmseg_ptr = shmat(myshmseg->segid, (char *) NULL, 0) )
					== (char *) NULL)
/* GEF */
/* Can't it in this order as we need a lock before pvm_shat() will work */
/* 			if ( pvm_shmat( myshmseg ) == (char *) NULL)	 */
/* we attach as this is the surest way of testing everything's ok */
/* GEF */
			{
				pvmlogprintf("Created segment but could not attach!\n");
				pvm_shmfree(myshmseg);
				myshmseg = (struct segdesc *) NULL;
				return PvmSysErr;
			}
			else
				shmdt (tmp_shmseg_ptr);
				/* we detach here to make sure that the attach count is right */
			break;
		}
		else
		{
			nretries ++;
			shmkey = (key_t) pvm_nextshmkey(shmkey);
		}
	}
	while (shmkey != first);

	if (!myshmseg)
	{
		pvmlogprintf("Could not create shared memory segment after %d\n",
				nretries);
		return PvmSysErr;
	}

	/* ==== Step 2: ===== */

	/* ops Phil... the key used for the shared memory segment may be used */
	/* by another semaphore.. so is should really be a loop like above */
	/* I know... it stinks, but... */
	lockkey = shmkey;

	if ( (lock = pvm_lockget(lockkey, myshmseg, LOCK_OWNER)) == 0)
	{
		pvmlogprintf("Could not create lock \n");
		return PvmSysErr;
	}

	/* ==== Step 2 and a half: ===== */
	/* ok... we can use the PVM_SHMAT routine now as we have a valid lock...  */

			if ( pvm_shmat( myshmseg ) == (char *) NULL)
			{
				pvmlogprintf("Created segment but could not attach!\n");
				pvm_shmfree(myshmseg);
				myshmseg = (struct segdesc *) NULL;
				return PvmSysErr;
			}

	/* ==== Step 3: ===== */
	/* Initialize the send/receive midlist */

	BZERO( pvm_shmmids, NSHMMIDS*sizeof(struct shm_midlist));

	*localseg = myshmseg;
	*key = shmkey;

	return 0;
}
