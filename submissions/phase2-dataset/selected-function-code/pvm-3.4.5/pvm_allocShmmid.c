pvm_allocShmmid(midentry)
struct shm_midlist **midentry;
{
int i;

	for (i = lastMidAlloced; i < NSHMMIDS; i++)
	{
		if (!pvm_shmmids[i].ml_status)  /* this mid is free */
		{
			pvm_shmmids[i].ml_status = ISALLOCED;
			lastMidAlloced = i;
			*midentry = pvm_shmmids + i;
			return i;
		}
	}

	for (i = 1; i < lastMidAlloced; i++)
	{
		if (!pvm_shmmids[i].ml_status)  /* this mid is free */
		{
			pvm_shmmids[i].ml_status = ISALLOCED;
			lastMidAlloced = i;
			*midentry = pvm_shmmids + i;
			return i;
		}
	}
	return PvmSysErr;		/* all the mids were full */

}
