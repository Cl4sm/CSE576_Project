int
pvm_archcode(arch)
	char *arch;
{
	struct pvmhostinfo *hlist;
	int sbf, rbf, cc;
	int nhost, narch;
	int i;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_ARCHCODE,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_AN, TEV_DATA_SCALAR,
				arch ? arch : "", 1, 1 );
			TEV_FIN;
		}
	}

	if (!arch)
		cc = PvmBadParam;

	else
	{
		/* Go get pvm_config() info...  :-Q :-Q :-Q */
		/* (can't use pvm_config() directly, as stomps */
		/* hostinfo structure...  Damn. */

		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			if (pvmschedtid)
				cc = msendrecv(pvmschedtid, SM_CONFIG, PvmBaseContext);
			else
				cc = msendrecv(TIDPVMD, TM_CONFIG, SYSCTX_TM);
			if (cc > 0) {
				pvm_upkint(&nhost, 1, 1);
				pvm_upkint(&narch, 1, 1);
				hlist = TALLOC(nhost, struct pvmhostinfo, "hi");
				for (i = 0; i < nhost; i++) {
					pvm_upkint(&hlist[i].hi_tid, 1, 1);
					pvmupkstralloc(&(hlist[i].hi_name));
					pvmupkstralloc(&(hlist[i].hi_arch));
					pvm_upkint(&hlist[i].hi_speed, 1, 1);
					pvm_upkint(&hlist[i].hi_dsig, 1, 1);
				}
				pvm_freebuf(pvm_setrbuf(rbf));
			}
			pvm_freebuf(pvm_setsbuf(sbf));
			pvm_setrbuf(rbf);

			cc = PvmNotFound;

			for ( i=0 ; i < nhost ; i++ ) {
				if ( !strcmp(hlist[i].hi_arch, arch) ) {
					cc = hlist[i].hi_dsig;
					break;
				}
			}

			while (nhost-- > 0) {
				PVM_FREE(hlist[nhost].hi_name);
				PVM_FREE(hlist[nhost].hi_arch);
			}
			PVM_FREE(hlist);
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_ARCHCODE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_AC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_archcode", cc);
	
	return( cc );
}
