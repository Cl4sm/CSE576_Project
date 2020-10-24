int
pvm_reg_rm(hip)
	struct pvmhostinfo **hip;
{
	int old_sched;
	int cc;
	int sbf;
	int rbf;
	/* char buf[256]; XXX static limit, argh (Not Any More! :-) JAK */
	static struct pvmhostinfo *hin = 0;
	static int mbindex = -1;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_REG_RM,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);

		if (pvmschedtid)
			cc = msendrecv(pvmschedtid, SM_SCHED, PvmBaseContext);
		else
			cc = msendrecv(TIDPVMD, TM_SCHED, SYSCTX_TM);
		if (cc > 0) {
			pvm_upkint(&cc, 1, 1);
			if (cc >= 0) {
				if (hin) {
					PVM_FREE(hin->hi_name);
					PVM_FREE(hin->hi_arch);

				} else
					hin = TALLOC(1, struct pvmhostinfo, "hi");
				pvm_upkint(&hin->hi_tid, 1, 1);
				pvmupkstralloc(&(hin->hi_name));
				pvmupkstralloc(&(hin->hi_arch));
				pvm_upkint(&hin->hi_speed, 1, 1);
				if (hip)
					*hip = hin;

				if ( mbindex >= 0 )
					pvm_delinfo( PVMRMCLASS, mbindex, PvmMboxDefault );
				pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
				pvm_pkint(&pvmmytid,1,1);
				mbindex = pvm_putinfo( PVMRMCLASS, pvm_getsbuf(),
					PvmMboxMultiInstance );
			}
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_REG_RM,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_reg_rm", cc);
	return cc;
}
