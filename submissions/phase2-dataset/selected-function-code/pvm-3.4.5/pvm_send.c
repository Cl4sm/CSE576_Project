int
pvm_send(tid, tag)
	int tid;	/* dest task */
	int tag;	/* type code */
{
	static struct timeval ztv = { 0, 0 };

	int tmp;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SEND,TEV_EVENT_ENTRY)) {
			int nb = -1;
			pvm_bufinfo(
					( pvmtrcsbfsave ) ? pvmtrcsbfsave : pvmsbuf->m_mid,
					&nb, (int *) NULL, (int *) NULL );
			TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR, &nb, 1, 1 );
			TEV_PACK_INT( TEV_DID_DST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if ( tid == PVM_TRACE_TID && tag == PVM_TRACE_CODE ) {
			if ( TEV_MASK_CHECK( pvmtrc.tmask, TEV_USER_DEFINED ) ) {
				/* Schlopp In End of Event Marker */
				enc_trc_fin(pvmsbuf);
				/* Send to Proper Tracer Destination */
				pvmsbuf->m_ctx = pvmtrc.trcctx;
				if ((cc = mroute(pvmsbuf->m_mid, pvmtrc.trctid,
						pvmtrc.trctag, &ztv)) > 0)
					cc = 0;
			}
			else
				cc = 0;
		}
		else if (!pvmrescode
				&& (!TIDISTASK(tid) || (tag & ~0x7fffffff)))
			cc = PvmBadParam;
		else
			if (!pvmsbuf)
				cc = PvmNoBuf;
			else {

/* XXX short-ckt to us should go here.  maybe can inc frag chain
   XXX count and make new message, put on pvmrxlist. */
				pvmsbuf->m_ctx = pvmmyctx;
				if ((cc = mroute(pvmsbuf->m_mid, tid, tag, &ztv)) > 0)
					cc = 0;
			}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SEND,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_send", cc);
	return cc;
}
