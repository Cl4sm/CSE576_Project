pvm_getopt(what)
	int what;
{
	int rc = 0;
	int err = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_GETOPT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_OPT, TEV_DATA_SCALAR, &what, 1, 1 );
			TEV_FIN;
		}
	}

	switch (what) {
	case PvmRoute:
		rc = pvmrouteopt;
		break;

	case PvmDebugMask:
		rc = pvmdebmask;
		break;

	case PvmAutoErr:
		rc = pvmautoerr;
		break;

	case PvmOutputTid:
		rc = pvmctrc.outtid;
		break;

	case PvmOutputContext:
		rc = pvmctrc.outctx;
		break;

	case PvmOutputCode:
		rc = pvmctrc.outtag;
		break;

	case PvmTraceTid:
		rc = pvmctrc.trctid;
		break;

	case PvmTraceContext:
		rc = pvmctrc.trcctx;
		break;

	case PvmTraceCode:
		rc = pvmctrc.trctag;
		break;

	case PvmTraceBuffer:
		rc = pvmctrc.trcbuf;
		break;

	case PvmTraceOptions:
		rc = pvmctrc.trcopt;
		break;

	case PvmFragSize:
		rc = pvmfrgsiz;
		break;

	case PvmResvTids:
		rc = pvmrescode;
		break;

	case PvmSelfOutputTid:
		rc = pvmtrc.outtid;
		break;

	case PvmSelfOutputContext:
		rc = pvmtrc.outctx;
		break;

	case PvmSelfOutputCode:
		rc = pvmtrc.outtag;
		break;

	case PvmSelfTraceTid:
		rc = pvmtrc.trctid;
		break;

	case PvmSelfTraceContext:
		rc = pvmtrc.trcctx;
		break;

	case PvmSelfTraceCode:
		rc = pvmtrc.trctag;
		break;

	case PvmSelfTraceBuffer:
		rc = pvmtrc.trcbuf;
		break;

	case PvmSelfTraceOptions:
		rc = pvmtrc.trcopt;
		break;

	case PvmShowTids:
		rc = pvmshowtaskid;
		break;

	case PvmNoReset:
		rc = pvmnoreset;
		break;

#if defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
	|| defined(IMA_ALPHAMP) || defined(IMA_CSPP) \
	|| defined(IMA_HPPAMP) || defined(IMA_RS6KMP) \
	|| defined(IMA_SGIMP) || defined(IMA_SGIMP6) \
	|| defined(IMA_SGIMP64) || defined(IMA_SUNMP)

	case PvmPollTime:
		rc = pvmpolltime;
		break;
	
	case PvmPollType:
		rc = pvmpolltype;
		break;

#else /* SHMEM */

	case PvmPollTime:
	case PvmPollType:
		rc = PvmNotImpl;
		err = 1;
		break;

#endif /* SHMEM */

	default:
		err = 1;
		break;
	}

	if (TEV_AMEXCL) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_GETOPT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_OPV, TEV_DATA_SCALAR, &rc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (err)
		return lpvmerr("pvm_getopt", PvmBadParam);
	return rc;
}
