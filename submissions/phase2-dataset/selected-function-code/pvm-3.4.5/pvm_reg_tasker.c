pvm_reg_tasker()
{
	static int imit = 0;		/* i'm the tasker */
	static int mbindex = -1;	/* mbox index */

	int cc;
	int sbf;
	int rbf;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_REG_TASKER,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);

		cc = imit ? 0 : 1;
		pvm_pkint(&cc, 1, 1);
		cc = msendrecv(TIDPVMD, TM_TASKER, SYSCTX_TM);
		if (cc > 0) {
			pvm_upkint(&cc, 1, 1);
			if (!cc) {
				imit = !imit;
				if ( imit ) {
					pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
					pvm_pkint(&pvmmytid,1,1);
					mbindex = pvm_putinfo( PVMTASKERCLASS,
						pvm_getsbuf(), PvmMboxMultiInstance );
				} else if ( mbindex >= 0 ) {
					if ( pvm_delinfo( PVMTASKERCLASS, mbindex,
							PvmMboxDefault ) >= 0 ) {
						mbindex = -1;
					}
				}
			}
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_REG_TASKER,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_reg_tasker", cc);
	return cc;
}
