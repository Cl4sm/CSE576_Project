int
pvm_addmhf(src, tag, ctx, f)
	int src, tag, ctx;			/* message source, context, tag */
#ifdef  IMA_SCO
	int (*f)(int);				/* handler function */
#else
	int (*f)();				/* handler function */
#endif
{
	int i;		/* temp local */
	int hand;	/* index into handles[] array -->> internal use only */
				/* - not for user */
	int mhid;	/* message handler index -->> into dhandles[] */
				/* -->> give to user */
	int exists = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_ADDMHF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &src, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR, &ctx, 1, 1 );
			TEV_FIN;
		}
	}

	/*
	*  confirm that does not already exist within "handles"
	*  this includes "wild-card" options in both directions too...
	*
	*  search from most recently added message handler to system
	*  added message handlers
	*/
	for ( i=nhandles-1 ; i >= 0 && !exists ; i-- ){
		if ((handles[i].header.tag == -1
				|| handles[i].header.tag == tag || tag == -1)
		&& (handles[i].header.ctx == -1
				|| handles[i].header.ctx == ctx || ctx == -1)
		&& (handles[i].header.src == -1
				|| handles[i].header.src == src || src == -1))
		{
			exists++;
		}
	}

	/*
	*  see if there are any already on free list
	*  -  use existing if on free list
	*  -  create new if none free - two scenarios:
	*       1. may be initial message handlers
	*       2. may be additional message handlers and lists just full
	*
	*  note: ndhandles never decreases - thus if less than nhandles,
	*  must be some free slots...
	*/
	if ( exists )
			mhid = PvmExists;
	else if (ndhandles > nhandles){
		/*
		*  there exist some free slots
		*/
		mhid = fl_dhandles;		/* take from head of free list */
		nhandles++;				/* incr actual # of active headers */

		/*
		*  move next one to head of free list
		*  if tail of list - will move "tail marker" (-1) to
		*  fl_dhandles to indicte empty list
		*/
		fl_dhandles = dhandles[mhid].mhid;

		/*
		*  fill the lists now...
		*/

		dhandles[mhid].mhid = mhid;		/* set to self */

		/* dhandles[mhid].handle	no change here as should be next */
		/* contiguous on handles[] */

		hand = dhandles[mhid].handle;	/* set to index -> handles[] */
		handles[hand].mhid = mhid;		/* set to corresponding mhid */
		handles[hand].header.tag = tag;
		handles[hand].header.ctx = ctx;
		handles[hand].header.src = src;
		handles[hand].f = f;	/* sonnofabeech! */
	}
	else{
		/* no free slots available - will have to allocate more slots...
		*  two scenarios for this:
		*    1. first time adding message handlers - none ever
		*           allocated.
		*    2. all presently allocated are filled and active - but
		*           been here before.
		*
		*  note: since no free slots, must have case of:
		*           ndhandles == nhandles
		*
		*  ndhandles : is the "high-water" mark as it is never decreased
		*  nhandles  : indicates the actual number of active handles
		*/
		if (ndhandles == 0){
			/* first time here - allocate new memory... */
			dhandles = TALLOC(++ndhandles, struct dhandler, "dhandles");
			handles = TALLOC(++nhandles, struct mhandler, "mhandle");
		}
		else{
			/* been here! - reallocate more space... */
			dhandles = TREALLOC(dhandles, ++ndhandles, struct dhandler);
			handles = TREALLOC(handles, ++nhandles, struct mhandler);
		}

		mhid = ndhandles - 1;		/* set index into dhandles */

		/*
		*  fill the lists now...
		*/

		dhandles[mhid].mhid = mhid;		/* set to self */
		dhandles[mhid].handle = mhid;	/* set same since lists full */

		handles[mhid].mhid = mhid;
		handles[mhid].header.tag = tag;
		handles[mhid].header.ctx = ctx;
		handles[mhid].header.src = src;
		handles[mhid].f = f;	/* sonnofabeech! */
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_ADDMHF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MHI, TEV_DATA_SCALAR, &mhid, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (mhid < 0)
		lpvmerr("pvm_addmhf", mhid);
	return mhid;	/* return message handler */
					/* -->> index into dhandles[] */
} /* pvm_addmhf */
