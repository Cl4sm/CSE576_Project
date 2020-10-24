pvm_notify(what, code, count, vals)
	int what;
	int code;
	int count;
	int *vals;
{
	static struct timeval ztv = { 0, 0 };

	int sbf;
	int cc;
	int numtid;
	int dosend = 0;
	struct waitc *wp;
	int flags;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_NOTIFY,TEV_EVENT_ENTRY)) {
			int tevcount;
			TEV_PACK_INT( TEV_DID_NE, TEV_DATA_SCALAR, &what, 1, 1 );
			TEV_PACK_INT( TEV_DID_NMC, TEV_DATA_SCALAR, &code, 1, 1 );
			tevcount = ( what != PvmHostAdd ) ? count : 0;
			TEV_PACK_INT( TEV_DID_NTL, TEV_DATA_ARRAY,
				vals, tevcount, 1 );
			TEV_FIN;
		}
	}

	flags = what;
	what &= ~PvmNotifyCancel;

	if (!(cc = BEATASK)) {
		if (!pvmrescode && (code & ~0x7fffffff)) {
			cc = PvmBadParam;

		} else {
			switch (what) {

			case PvmHostDelete:
				if (count < 1)
					cc = PvmBadParam;
				else {
					numtid = count;
					dosend = 1;
				}
				break;

			case PvmTaskExit:
				if (count < 1)
					cc = PvmBadParam;
				else
					for (numtid = count; numtid-- > 0; )
						if (!TIDISTASK(vals[numtid])) {
							cc = PvmBadParam;
							break;
						}
				if (!cc) {
					for (numtid = count; numtid-- > 0; )
						if (vals[numtid] == pvmmytid) {
							wp = wait_new(WT_TASKX);
							wp->wa_tid = pvmmytid;
							wp->wa_on = pvmmytid;
							sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
							pvm_pkint(&pvmmytid, 1, 1);
							sbf = pvm_setsbuf(sbf);
							wp->wa_mesg = midtobuf(sbf);
							wp->wa_mesg->m_ctx = pvmmyctx;
							wp->wa_mesg->m_tag = code;
						}
					numtid = count;
					dosend = 1;
				}
				break;

			case PvmHostAdd:
				numtid = 0;
				vals = &numtid;
				dosend = 1;
				break;

			case PvmRouteAdd:
				FORLIST (wp, waitlist, wa_link)
					if (wp->wa_kind == WT_ROUTEA
					&& wp->wa_mesg->m_ctx == pvmmyctx
					&& wp->wa_mesg->m_tag == code)
						break;

				if (count == 0 || (flags & PvmNotifyCancel)) {
					if (wp != waitlist)
						wait_delete(wp);

				} else {
					if (wp == waitlist) {
						wp = wait_new(WT_ROUTEA);
						wp->wa_tid = pvmmytid;
						wp->wa_on = pvmmytid;
						sbf = pvm_mkbuf(PvmDataFoo);
						wp->wa_mesg = midtobuf(sbf);
						wp->wa_mesg->m_ctx = pvmmyctx;
						wp->wa_mesg->m_tag = code;
					}
					wp->wa_count = count;
				}
				break;

			case PvmRouteDelete:
				if (count < 1)
					cc = PvmBadParam;
				else
					for (numtid = count; numtid-- > 0; )
						if (!TIDISTASK(vals[numtid])) {
							cc = PvmBadParam;
							break;
						}
				if (!cc) {
					for (numtid = count; numtid-- > 0; )
						post_routedelete(vals[numtid], pvmmyctx, code);
				}
				break;

			default:
				cc = PvmBadParam;
				break;
			}

			if (dosend) {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				pvm_pkint(&flags, 1, 1);
				pvm_pkint(&pvmmyctx, 1, 1);
				pvm_pkint(&code, 1, 1);
				pvm_pkint(&count, 1, 1);
				pvm_pkint(vals, numtid, 1);

				if (pvmschedtid)
					cc = mroute(pvmsbuf->m_mid, pvmschedtid, SM_NOTIFY, &ztv);
				else
					cc = mroute(pvmsbuf->m_mid, TIDPVMD, TM_NOTIFY, &ztv);
				pvm_freebuf(pvm_setsbuf(sbf));
				if (cc > 0)
					cc = 0;
			}
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_NOTIFY,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_notify", cc);
	return cc;
}
