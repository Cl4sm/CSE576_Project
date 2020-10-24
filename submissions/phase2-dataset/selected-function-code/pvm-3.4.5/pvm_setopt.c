pvm_setopt(what, val)
	int what;
	int val;
{
	int rc = 0;
	int err = 0;
	int sbf, rbf;
	char buf[16];
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_SETOPT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_OPT, TEV_DATA_SCALAR, &what, 1, 1 );
			TEV_PACK_INT( TEV_DID_OPV, TEV_DATA_SCALAR, &val, 1, 1 );
			TEV_FIN;
		}
	}

	switch (what) {
	case PvmRoute:
		switch (val) {
		case PvmDontRoute:
		case PvmAllowDirect:
		case PvmRouteDirect:
			rc = pvmrouteopt;
#if (!defined(IMA_PGON)) && (!defined(IMA_SP2MPI)) \
		&& (!defined(IMA_AIX4SP2)) && (!defined(IMA_AIX5SP2))
			pvmrouteopt = val;
#endif
			break;

		default:
			rc = PvmBadParam;
			err = 1;
			break;
		}
		break;

	case PvmDebugMask:
		rc = pvmdebmask;
		pvmdebmask = val;
		break;

	case PvmAutoErr:
		rc = pvmautoerr;
		pvmautoerr = val;
		break;

	case PvmOutputTid:
		if (val && val != pvmmytid
		&& (val != pvmtrc.outtid || pvmctrc.outtag != pvmtrc.outtag)) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.outtid;
			pvmctrc.outtid = val;
		}
		break;

	case PvmOutputContext:
		if (pvmctrc.outtid > 0 && pvmctrc.outtid != pvmmytid
		&& val != pvmtrc.outctx) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.outctx;
			pvmctrc.outctx = val;
		}
		break;

	case PvmOutputCode:
		if (pvmctrc.outtid > 0 && pvmctrc.outtid != pvmmytid
		&& val != pvmtrc.outtag) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.outtag;
			pvmctrc.outtag = val;
		}
		break;

	case PvmTraceTid:
		if (val && val != pvmmytid
		&& (val != pvmtrc.trctid || pvmctrc.trctag != pvmtrc.trctag)) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.trctid;
			pvmctrc.trctid = val;
		}
		break;

	case PvmTraceContext:
		if (pvmctrc.trctid > 0 && pvmctrc.trctid != pvmmytid
		&& val != pvmtrc.trcctx) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.trcctx;
			pvmctrc.trcctx = val;
		}
		break;

	case PvmTraceCode:
		if (pvmctrc.trctid > 0 && pvmctrc.trctid != pvmmytid
		&& val != pvmtrc.trctag) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.trctag;
			pvmctrc.trctag = val;
		}
		break;

	case PvmTraceBuffer:
		rc = pvmctrc.trcbuf;
		pvmctrc.trcbuf = val;
		break;

	case PvmTraceOptions:
		switch (val) {
		case PvmTraceFull:
		case PvmTraceTime:
		case PvmTraceCount:
			rc = pvmctrc.trcopt;
			pvmctrc.trcopt = val;
			break;
		
		default:
			rc = PvmBadParam;
			err = 1;
			break;
		}
		break;

	case PvmFragSize:

#if defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
	|| defined(IMA_ALPHAMP) || defined(IMA_CSPP) \
	|| defined(IMA_HPPAMP) || defined(IMA_RS6KMP) \
	|| defined(IMA_SGIMP) || defined(IMA_SGIMP6) \
	|| defined(IMA_SGIMP64) || defined(IMA_SUNMP)

		rc = PvmBadParam;
		err = 1;

#else /* SHMEM */

#if defined(IMA_CM5) || defined(IMA_CUBE) || defined(IMA_I860) \
	|| defined(IMA_PGON) || defined(IMA_SP2MPI) \
	|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
	|| defined(IMA_BEOLIN)
		/* if (val < TDFRAGHDR + TTMSGHDR + 4 || val > 1048576) */
		if (val < TDFRAGHDR + MSGHDRLEN + 4 || val > MAXFRAGSIZE)
#else
		if (val < TDFRAGHDR + MSGHDRLEN + 4 || val > 1048576)
#endif

		{
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmfrgsiz;
			pvmfrgsiz = val;
		}

#endif /* SHMEM */

		break;

	case PvmResvTids:
		rc = pvmrescode;
		pvmrescode = val;
		break;

	case PvmSelfOutputTid:
		if (pvmtrc.outtid > 0 || val > 0) {
			if (rc = BEATASK)
				err = 1;
			else {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				rbf = pvm_setrbuf(0);
				what = TS_OUTTID;
				pvm_pkint(&what, 1, 1);
				sprintf(buf, "%x", 0xffffffff & val);
				pvm_pkstr(buf);
				if ((rc = msendrecv(TIDPVMD, TM_SETOPT, SYSCTX_TM)) > 0) {
					pvm_freebuf(pvm_setrbuf(rbf));
					rc = pvmtrc.outtid;
					pvmtrc.outtid = val;
					pvmctrc.outtid = pvmtrc.outtid;
					pvmctrc.outctx = pvmtrc.outctx;
					pvmctrc.outtag = pvmtrc.outtag;

				} else {
					pvm_setrbuf(rbf);
					err = 1;
				}
				pvm_freebuf(pvm_setsbuf(sbf));
			}
		}
		else {
			rc = pvmtrc.outtid;
			pvmtrc.outtid = val;
			pvmctrc.outtid = pvmtrc.outtid;
			pvmctrc.outctx = pvmtrc.outctx;
			pvmctrc.outtag = pvmtrc.outtag;
		}
		break;

	case PvmSelfOutputContext:
		rc = PvmNotImpl;
		err = 1;
		break;

	case PvmSelfOutputCode:
		if (pvmtrc.outtid <= 0 || (rc = BEATASK))
			err = 1;
		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			what = TS_OUTTAG;
			pvm_pkint(&what, 1, 1);
			sprintf(buf, "%x", 0xffffffff & val);
			pvm_pkstr(buf);
			if ((rc = msendrecv(TIDPVMD, TM_SETOPT, SYSCTX_TM)) > 0) {
				pvm_freebuf(pvm_setrbuf(rbf));
				rc = pvmtrc.outtag;
				pvmtrc.outtag = val;
				pvmctrc.outtid = pvmtrc.outtid;
				pvmctrc.outctx = pvmtrc.outctx;
				pvmctrc.outtag = pvmtrc.outtag;

			} else {
				pvm_setrbuf(rbf);
				err = 1;
			}
			pvm_freebuf(pvm_setsbuf(sbf));
		}
		break;

	case PvmSelfTraceTid:
		if ( pvmtrc.trctid > 0 || val > 0 ) {
			if (rc = BEATASK)
				err = 1;
			else {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				rbf = pvm_setrbuf(0);
				what = TS_TRCTID;
				pvm_pkint(&what, 1, 1);
				sprintf(buf, "%x", 0xffffffff & val);
				pvm_pkstr(buf);
				if ((rc = msendrecv(TIDPVMD, TM_SETOPT, SYSCTX_TM)) > 0) {
					pvm_freebuf(pvm_setrbuf(rbf));
					rc = pvmtrc.trctid;
					pvmtrc.trctid = val;
					pvmctrc.trctid = pvmtrc.trctid;
					pvmctrc.trcctx = pvmtrc.trcctx;
					pvmctrc.trctag = pvmtrc.trctag;

				} else {
					pvm_setrbuf(rbf);
					err = 1;
				}
				pvm_freebuf(pvm_setsbuf(sbf));
			}
		}
		else {
			rc = pvmtrc.trctid;
			pvmtrc.trctid = val;
			pvmctrc.trctid = pvmtrc.trctid;
			pvmctrc.trcctx = pvmtrc.trcctx;
			pvmctrc.trctag = pvmtrc.trctag;
		}
		break;

	case PvmSelfTraceContext:
		rc = PvmNotImpl;
		err = 1;
		break;

	case PvmSelfTraceCode:
		if (pvmtrc.trctid <= 0 || (rc = BEATASK))
			err = 1;
		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			what = TS_TRCTAG;
			pvm_pkint(&what, 1, 1);
			sprintf(buf, "%x", 0xffffffff & val);
			pvm_pkstr(buf);
			if ((rc = msendrecv(TIDPVMD, TM_SETOPT, SYSCTX_TM)) > 0) {
				pvm_freebuf(pvm_setrbuf(rbf));
				rc = pvmtrc.trctag;
				pvmtrc.trctag = val;
				pvmctrc.trctid = pvmtrc.trctid;
				pvmctrc.trcctx = pvmtrc.trcctx;
				pvmctrc.trctag = pvmtrc.trctag;

			} else {
				pvm_setrbuf(rbf);
				err = 1;
			}
			pvm_freebuf(pvm_setsbuf(sbf));
		}
		break;

	case PvmSelfTraceBuffer:
		tev_flush( 1 );
		rc = pvmtrc.trcbuf;
		pvmtrc.trcbuf = val;
		break;

	case PvmSelfTraceOptions:
		tev_flush( 1 );
		switch (val) {
		case PvmTraceFull:
		case PvmTraceTime:
		case PvmTraceCount:
			rc = pvmtrc.trcopt;
			pvmtrc.trcopt = val;
			break;

		default:
			rc = PvmBadParam;
			err = 1;
			break;
		}
		break;

	case PvmShowTids:
		rc = pvmshowtaskid;
		pvmshowtaskid = val;
		break;

	case PvmNoReset:
		if ( pvmmytid < 0 ) {
			rc = PvmBadParam;
			lpvmerr("pvm_setopt: PvmNoReset task not initialized", rc);
			err = 1;
		} else {
			rc = pvmnoreset;
			if ( val && !pvmnoreset ) {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
				pvm_pkint( &pvmmytid, 1, 1 );
				if ( pvmnoresetindex = pvm_putinfo( PVMNORESETCLASS,
						pvm_getsbuf(), PvmMboxMultiInstance ) < 0 ) {
					rc = PvmSysErr;
					err = 1;
					val = 0;
				}
				pvm_freebuf(pvm_setsbuf(sbf));
			}
			else if ( !val && pvmnoreset && pvmnoresetindex >= 0 ) {
				if ( pvm_delinfo( PVMNORESETCLASS, pvmnoresetindex,
						PvmMboxDefault ) < 0 ) {
					rc = PvmSysErr;
					err = 1;
					val = 0;
				}
				pvmnoresetindex = -1;
			}
			pvmnoreset = val;
		}
		break;

#if defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
	|| defined(IMA_ALPHAMP) || defined(IMA_CSPP) \
	|| defined(IMA_HPPAMP) || defined(IMA_RS6KMP) \
	|| defined(IMA_SGIMP) || defined(IMA_SGIMP6) \
	|| defined(IMA_SGIMP64) || defined(IMA_SUNMP)

	case PvmPollTime:
		rc = pvmpolltime;
		if (val < 0) {
			rc = PvmBadParam;
			err = 1;
			break;
		}
		pvmpolltime = val;
		break;

	case PvmPollType:
		rc = pvmpolltype;
		if ((val != PvmPollConstant) && (val != PvmPollSleep)) {
			rc = PvmBadParam;
			err = 1;
			break;
		}
		pvmpolltype = val;
		break;

#else /* SHMEM */

	case PvmPollTime:
	case PvmPollType:
		rc = PvmNotImpl;
		err = 1;
		break;

#endif /* SHMEM */

	default:
		rc = PvmBadParam;
		err = 1;
		break;
	}

	if (TEV_AMEXCL) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_SETOPT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_OPV, TEV_DATA_SCALAR, &rc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (err)
		return lpvmerr("pvm_setopt", rc);
	return rc;
}
