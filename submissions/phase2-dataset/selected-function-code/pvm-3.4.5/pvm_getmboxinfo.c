int
pvm_getmboxinfo(pattern, nclasses, classes)
	char *pattern;					/* class name */
	int *nclasses;					/* number of classes returned */
	struct pvmmboxinfo **classes;	/* name list returned */
{
	static struct pvmmboxinfo *clist = (struct pvmmboxinfo *) NULL;
	static int nclist = 0;

	int sbf, rbf, cc;
	int i, j;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETMBOXINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				pattern ? pattern : "", 1, 1 );
			TEV_FIN;
		}
	}

	if ( !pattern )
		pattern = "";

	if ( clist ) {
		for ( i=0 ; i < nclist ; i++ ) {
			if ( clist[i].mi_name )
				PVM_FREE( clist[i].mi_name );
			if ( clist[i].mi_indices )
				PVM_FREE( clist[i].mi_indices );
			if ( clist[i].mi_owners )
				PVM_FREE( clist[i].mi_owners );
			if ( clist[i].mi_flags )
				PVM_FREE( clist[i].mi_flags );
		}
		PVM_FREE( clist );
		clist = (struct pvmmboxinfo *) NULL;
		nclist = 0;
	}

	if ( !(cc = BEATASK) )
	{
		sbf = pvm_setsbuf( pvm_mkbuf( PvmDataFoo ) );
		rbf = pvm_setrbuf( 0 );
		cc = TMDB_NAMES;
		pvm_pkint( &cc, 1, 1 );
		pvm_pkint( &pvmmytid, 1, 1 );
		pvm_pkstr( pattern );
		cc = 0;
		pvm_pkint( &cc, 1, 1 );
		pvm_pkint( &cc, 1, 1 );
		if ( (cc = msendrecv( TIDPVMD, TM_DB, SYSCTX_TM )) > 0 ) {
			pvm_upkint( &cc, 1, 1 );
			if ( cc >= 0 ) {
				pvm_upkint( &nclist, 1, 1 );
				clist = TALLOC( nclist, struct pvmmboxinfo, "classes" );
				for ( i=0 ; i < nclist ; i++ ) {
					pvmupkstralloc( &(clist[i].mi_name) );
					pvm_upkint( &(clist[i].mi_nentries), 1, 1 );
					clist[i].mi_indices = TALLOC( clist[i].mi_nentries,
							int, "class_indices" );
					clist[i].mi_owners = TALLOC( clist[i].mi_nentries,
							int, "class_owners" );
					clist[i].mi_flags = TALLOC( clist[i].mi_nentries,
							int, "class_flags" );
					for ( j=0 ; j < clist[i].mi_nentries ; j++ ) {
						pvm_upkint( &(clist[i].mi_indices[j]), 1, 1 );
						pvm_upkint( &(clist[i].mi_owners[j]), 1, 1 );
						pvm_upkint( &(clist[i].mi_flags[j]), 1, 1 );
					}
				}
				if (classes)
					*classes = clist;
				if (nclasses)
					*nclasses = nclist;
			}
			pvm_freebuf(pvm_setrbuf(rbf));
		}
		
		else
			pvm_setrbuf(rbf);

		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETMBOXINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_getmboxinfo", cc);
	return cc;
}
