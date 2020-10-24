pvm_tasks(where, ntaskp, taskp)
	int where;					/* which host or 0 for all */
	int *ntaskp;
	struct pvmtaskinfo **taskp;
{
	int cc, ec, sbf, rbf, ae;
	static struct pvmtaskinfo *tlist = 0;
	static int ntask = 0;
	int len1 = 5, len2 = 3;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_TASKS,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TW, TEV_DATA_SCALAR, &where, 1, 1 );
			TEV_FIN;
		}
	}

	if (tlist) {
		while (ntask-- > 0)
			PVM_FREE(tlist[ntask].ti_a_out);
		PVM_FREE(tlist);
		tlist = 0;
		ntask = 0;
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		pvm_pkint(&where, 1, 1);

		if (pvmschedtid)
			cc = msendrecv(pvmschedtid, SM_TASK, PvmBaseContext);
		else
			cc = msendrecv(TIDPVMD, TM_TASK, SYSCTX_TM);
		if (cc > 0) {
			if (!(cc = pvm_upkint(&ec, 1, 1))
			&& (cc = ec) >= 0) {
				tlist = TALLOC(len1, struct pvmtaskinfo, "ti");
				ae = pvm_setopt(PvmAutoErr, 0);
				ntask = 0;
				while (!pvm_upkint(&tlist[ntask].ti_tid, 1, 1)) {
					pvm_upkint(&tlist[ntask].ti_ptid, 1, 1);
					pvm_upkint(&tlist[ntask].ti_host, 1, 1);
					pvm_upkint(&tlist[ntask].ti_flag, 1, 1);
					pvmupkstralloc(&(tlist[ntask].ti_a_out));
					pvm_upkint(&tlist[ntask].ti_pid, 1, 1);
					ntask++;
					if (ntask == len1) {
						len1 += len2;
						len2 = ntask;
						tlist = TREALLOC(tlist, len1, struct pvmtaskinfo);
					}
				}
				pvm_setopt(PvmAutoErr, ae);
				cc = 0;
			}
			pvm_freebuf(pvm_setrbuf(rbf));
			if (ntaskp)
				*ntaskp = ntask;
			if (taskp)
				*taskp = tlist;
		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_TASKS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_PACK_INT( TEV_DID_TNT, TEV_DATA_SCALAR, &ntask, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_tasks", cc);
	return cc;
}
