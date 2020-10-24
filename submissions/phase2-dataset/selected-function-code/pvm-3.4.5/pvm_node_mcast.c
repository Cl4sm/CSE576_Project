pvm_node_mcast(tids, count, code, offhtids, offhcnt)
int *tids; 	/* array of tids, on and off host */
int count;	/* #of tids */
int code;	/* msgtag tag */
int **offhtids; /* array of tids going off host, returns null if none */
int *offhcnt; /* # of tids going off host, returns 0 if none */
{

	int i;
	int mask = pvmtidhmask;		/* host */
	int nlocal = 0;
	int localidx = count;
	int nremote = 0;
	int *tmptids = (int *) NULL;
	*offhtids = (int *) NULL;
	*offhcnt = 0;	

	if (!tids || count < 0) 		/* no tids, or bogus count */
		return PvmOk;

	if (! (tmptids = TALLOC(count, int, "nmcast")))
	{
		pvmlogerror("node_mcast(): couldn't alloc memory\n");
		return PvmNoMem;
	}

	for (i = 0; i < count; i++)
	{
		if ( TIDISNODE(tids[i]) && (tids[i] & mask) == (pvmmytid & mask))
		{
			/* put the local tids starting at the back of the array */
			tmptids[--localidx] = tids[i];	/* tid local to us */
			nlocal ++;
		}
		else
		{
			/* put non-local tids at the front of the array */
			tmptids[nremote++] = tids[i];
		}

	}

	/* multicast to the local tasks */
	for (i = 0; i < nlocal; i++)
		pvm_send(tmptids[localidx++], code);      /* loop of sends for now */

	if (nlocal == count)
	{
		PVM_FREE(tmptids);
		tmptids = (int *) NULL;
	}
	else
	{
		*offhtids = tmptids;
		*offhcnt = nremote;	
	}
	
	return PvmOk;	
}
