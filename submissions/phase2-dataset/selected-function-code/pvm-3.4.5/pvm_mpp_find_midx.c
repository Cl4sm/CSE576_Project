int
pvm_mpp_find_midx(mppsendmids, mppoutchunks, current, num, mfunc )
msgmid_t *mppsendmids;
CHUNK_PTR *mppoutchunks;
int *current;
int num;
MSGFUNC_PTR mfunc;
{
int foundidx;
int i, idx;
int appid = 0;			/** XXXX fix this for PGONPUMA */
info_t minfo[MPPINFOSIZE];

	foundidx = 0;

	idx = *current;
	

	for (i = 0; i < num; i++ )
	{

		/* Check if a previous send has finished */
		if (mppsendmids[idx] >= 0)
		{
			if ((*mfunc->msgdone)(appid, &(mppsendmids[idx]), minfo) )
			{
				mppsendmids[idx] = (msgmid_t) -1; 
				if (mppoutchunks[idx])
				{
					CHUNK_FREE(mppoutchunks[idx]);		
					mppoutchunks[idx] = (CHUNK_PTR) NULL;
				}
				foundidx = 1;
			}
		}
		else 
		{
			if (mppsendmids[idx] == (msgmid_t) MPPMIDFREE)
				foundidx = 1;
		}
		
		if (foundidx)
			break;

		if (++idx >= num)
			 idx = 0;
	}

	if (foundidx)
	{
		mppsendmids[idx] = (msgmid_t) MPPMIDALLOCED;

		if ( (*current = idx + 1) >= num)
			*current = 0;

		return (idx);
	}
	else
		return (-1);
}
