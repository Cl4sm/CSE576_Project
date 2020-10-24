int
pvm_node_send(cp, len, ttpcbp, smsglist, inPlaceHeader, inPlaceBodyLen)
	char *cp;				/* this is what we are supposed to be sending */
	int len;				/* this is the length of the frag */
	struct ttpcb * ttpcbp;	/* info about where this is going */
	struct msgid **smsglist; /* msgid for this send */
	char *inPlaceHeader;	/* This is an inplace header? */
	int inPlaceBodyLen;		/* Length of inplace fragment */
{
	int cc;
	int dtid;
	int mask;						/* used to mask different parts of tid */
	msgmid_t mid;
	int node;						/* destination node */
	int partid;						/* destination partition id */
	int tag;


	MSGFUNC_PTR	mfunc;	

#if defined(IMA_PGON)
	double dclock();
#endif
	static double lastClock;
	double delay;

	static int savelen = -1;/* save the length of inplace body for next call */
	
	MPP_DIRECTI_PTR tdirect;

	struct msgid *cmsgid = (struct msgid *) NULL;

	mask = TIDHOST;

	dtid = ttpcbp -> tt_tid;

	if (TIDISNODE(dtid) && (dtid & pvmtidhmask) == (pvmmytid & pvmtidhmask)
		&& (dtid & pvmtidpmask) == (pvmmytid & pvmtidpmask)) 
	{
		node = dtid & pvmtidnmask;
		partid = pvmmyptype;			/* send to node directly */
		mfunc = pvm_nodemsgfunc();		/* point to node-node routines */
	} 
	else 
	{
		node = pvmhostnode;
		partid = PVMDPTYPE;			/* send to pvmd first */
		mfunc = pvm_hostmsgfunc();		/* point to host-node routines */
	}

	
	tdirect = ttpcbp->mpdirect;

	if (inPlaceHeader)
	{
		savelen = inPlaceBodyLen;
#if defined(IMA_PGON)
		if (savelen > 4096 )
		{
			lastClock = dclock();	
			delay = (double) inplaceDelay / 1e6;
		}
#endif
	}

	if ( savelen >= 0  && inPlaceHeader == (char *) NULL) 
	{
		tag = pvmmynode;		/* send inplace body with my physical node # */
#if defined(IMA_PGON)
		if (savelen > 4096 )
			while ((dclock() - lastClock) < delay); 
#endif
		savelen = -1;		  	/* as the tag */
	}
	else
	{
		tag = tdirect->tagbase + tdirect->sseq;

		if (++(tdirect->sseq) >= tdirect->nbufs) 
				tdirect->sseq = 0;
	}

	{
		if ((cc = (*mfunc->imsgsend)(0,tag, cp, len, node, partid, &mid))
				&& (cc < 0)) 
		{
			pvmlogperror("node_send() IMSGSEND \n ");
			return PvmSysErr;
		}

		/* can't merge id's  add to the list */
		if (!(mfunc->msgmerge)  || !(*smsglist)) 
		{
			if ( (cmsgid = msgid_new()) == (struct msgid *) NULL)
			{
				pvmlogerror("node_send(): couldn't allocate send msgid\n");
				return PvmSysErr;
			}
			if (! (*smsglist))  	/* intialize the list */
			{
				*smsglist = cmsgid;
				cmsgid->ms_link = cmsgid->ms_rlink = cmsgid;
			}
			else				/* put at the end of the list of message id's */
				LISTPUTBEFORE(*smsglist, cmsgid, ms_link, ms_rlink);
		}
		else
		{
			cmsgid = *smsglist;
			cmsgid -> id = (*mfunc->msgmerge)(&(cmsgid->id), &mid);
		}

		if (cmsgid -> otid != node)
		{
			cmsgid -> id = mid;
			cmsgid -> otid = node;
			cmsgid -> complete = 0;
			cmsgid -> len += len;
			cmsgid -> mfunc = mfunc;
		}
	
	}
	return len;
}
