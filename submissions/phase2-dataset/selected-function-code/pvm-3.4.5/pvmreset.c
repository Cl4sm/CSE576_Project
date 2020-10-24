pvmreset( mytid, killtasks, class, index )
int mytid;
int killtasks;
char *class;
int index;
{
	struct pvmtaskinfo *tip;
	int ntask;

	int *noresets = (int *) NULL;
	int nnr = 0;

	int sbf, rbf;
	int found;
	int i, j;
	int cc;

	if ( !pvm_tasks( 0, &ntask, &tip ) && ntask > 0 )
	{
		pvm_getnoresets( &noresets, &nnr );

		for ( i=0 ; i < ntask && killtasks ; i++ )
		{
			for ( j=0, found=0 ; j < nnr && !found ; j++ )
				if ( noresets[j] == tip[i].ti_tid )
					found++;

			if ( !found && tip[i].ti_tid && tip[i].ti_tid != mytid )
				pvm_kill(tip[i].ti_tid);
		}
	}

	/* XXX this is gnasty... */
	/* while ((i = pvm_lookup("pvmgs", -1, &j)) >= 0) */
		/* pvm_delete("pvmgs", i); */

	/* Tell Master PVMD to clean up mboxes... */

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	rbf = pvm_setrbuf(0);
	cc = TMDB_RESET;
	pvm_pkint(&cc, 1, 1);
	pvm_pkint(&pvmmytid, 1, 1);
	pvm_pkstr(class ? class : "");
	cc = 0;
	pvm_pkint(&index, 1, 1);
	pvm_pkint(&killtasks, 1, 1);
	pvm_pkint(&nnr, 1, 1);
	for ( i=0 ; i < nnr ; i++ )
		pvm_pkint(&(noresets[i]), 1, 1);
	if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
		pvm_upkint(&cc, 1, 1);
		pvm_freebuf(pvm_setrbuf(rbf));
	} else
		pvm_setrbuf(rbf);
	pvm_freebuf(pvm_setsbuf(sbf));

	return 0;
}
