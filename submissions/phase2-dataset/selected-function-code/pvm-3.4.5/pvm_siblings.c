int
pvm_siblings(tidsp)
	int **tidsp;
{
	static int pvmsiblings_me[1];
	int cc = PvmParentNotSet;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SIBLINGS,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	if (pvmnsibs == -1)  /* only call pvm_parent if we haven't gotten sibs */
		cc = pvm_parent();

	if (cc > 0 || cc == PvmParentNotSet) 
	{
		/* wait for pvmnsibs to be set by spawning program */ 
		while (pvmnsibs == -1) {
			cc = mroute(0, 0, 0, (struct timeval *)0);
			if (cc < 0)
				break;
		}
		if (pvmnsibs != -1) {
			cc = pvmnsibs;
			*tidsp = pvmsibtids;
		}

	} else 
		if (cc == PvmNoParent)  /* I am my only sibling */
		{
			cc = 1;
			pvmsiblings_me[0] = pvmmytid;
			*tidsp = pvmsiblings_me; 
		}
			
			

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SIBLINGS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_SIB, TEV_DATA_ARRAY,
				pvmsibtids, cc, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_siblings", cc);
	return cc;
}
