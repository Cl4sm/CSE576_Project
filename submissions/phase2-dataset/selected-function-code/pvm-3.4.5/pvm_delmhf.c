pvm_delmhf(mhid)
	int mhid;			/* message handler index */
{
	char *errstr;
	int htoid;			/* handle - moved to   - id location */
	int hfrid;			/* handle - moved from - id location */
	int rc = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_DELMHF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MHI, TEV_DATA_SCALAR, &mhid, 1, 1 );
			TEV_FIN;
		}
	}

	/*
	*  check incomming parameter at lower and upper boundary
	*	- can't have negative mhid values
	*	- too high an index - mhid entry does not exist
	*/
	if (mhid < 0) { /* too low */
		errstr = "pvm_delmhf";
		rc = PvmBadParam;
	}
	else if (mhid >= ndhandles) { /* too high */
		errstr = "(mhid >= ndhandles) pvm_delmhf";
		rc = PvmNotFound;
	}

	/*
	*  make sure is not already on free list - can only delete active
	*  mhid entries
	*
	*  confirm active by:
	*    since handles is contiguous at low order indices, thus any
	*    entry in dhandles[x].handle less-than nhandles is an active
	*    entry and may be deleted
	*/
	if ( rc >= 0 ) {
		if ( dhandles[mhid].handle < nhandles ) {
			/*
			*  this one's active - delete it
			*/

			nhandles--;		/* reduce # of active handles[] entries */
			htoid = dhandles[mhid].handle;	/* handle - moved to */
											/* -id location */
			hfrid = nhandles;		/* handle - moved from - id loc */

			/* Don't Spank List if Last Entry...  D-Oh! */
			if ( htoid != hfrid )
			{
				/*
				*  move the last "active" handle[] entry up to "deleted"
				*  location
				*/

				handles[htoid] = handles[hfrid];	/* the move */

				/* unnecessary?! handles[hfrid].mhid = htoid; */
				/* ("back-link" ptr adjust?) */

				/*
				*  adjust dhandles[] to point to
				*    1. the moved "up" entry
				*    2. the "deleted" entry
				*/

				/* 1. the moved "up" entry */
				dhandles[handles[htoid].mhid].handle = htoid;

				/* 2. the "deleted" entry */
				dhandles[mhid].handle = hfrid;
			}

			/*
			*  put the "freed" dhandle entry (mhid value) on head of
			*  the free list
			*/

			/* link this to previous free list */
			dhandles[mhid].mhid = fl_dhandles;

			fl_dhandles = mhid;		/* put this one on head of list */
		}
		else {
			/* this one is already on free list - not active entry */
			errstr = "pvm_delmhf";
			rc = PvmNotFound;
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_DELMHF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &rc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (rc < 0)
		lpvmerr(errstr, rc);
	return( rc );

} /* pvm_delmhf */
