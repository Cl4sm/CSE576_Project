int
pvm_lockget (key, sseg, iAmOwner )
key_t key;
struct segdesc *sseg;
int iAmOwner;
{
int flags;
int semid;
/* GEF */
/* ops SysV requires that we explicitly declare this if the forth arg is needed
 * in any shmem op.
 */
/* union semun lockarg; */
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
} lockarg;

int lockcmd;

	if ( sseg == (struct segdesc *) NULL)
	{
		pvmlogerror( "pvm_lockget (): invalid segment descriptor \n");
		return PvmBadParam;
	}

	if (iAmOwner)
		flags =IPC_CREAT | IPC_EXCL | PERMS;
	else
		flags = PERMS;

	if ((sseg -> lock = semget(key, 1, flags)) == -1
		 && pvmdebmask& PDMNODE)
	{
		sseg -> lock = 0;
		pvmlogperror( "pvm_lockget (): could not create lock \n");
		return 0;
	}

	sseg -> pagelock =  pvm_shmlock;
	sseg -> pageunlock = pvm_shmunlock;

/* If I am the owner, initialize the lock to 0 (unlocked) */

	lockcmd = IPC_SET;
	lockarg.val = 0;
	semctl( sseg -> lock, 0, lockcmd, lockarg );

	return sseg -> lock;
}
