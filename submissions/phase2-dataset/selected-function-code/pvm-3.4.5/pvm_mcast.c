int
pvm_mcast(tids, count, tag)
	int *tids;		/* dest tasks */
	int count;		/* number of tids */
	int tag;		/* message type tag */
{
	int cc;			/* for return codes */
	int i;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_MCAST,TEV_EVENT_ENTRY)) {
			int nb = -1;
			pvm_bufinfo(
					( pvmtrcsbfsave ) ? pvmtrcsbfsave : pvmsbuf->m_mid,
					&nb, (int *) NULL, (int *) NULL );
			TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR, &nb, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_PACK_INT( TEV_DID_MDL, TEV_DATA_ARRAY, tids, count, 1 );
			TEV_FIN;
		}
	}

	/* sanity check args and sendable message */

	if (!(cc = BEATASK)) {
		if (!pvmsbuf)
			cc = PvmNoBuf;
		else if (count < 0 || (!pvmrescode && (tag & ~0x7fffffff)))
				cc = PvmBadParam;
		else if (!pvmrescode)
			for (i = count; i-- > 0; )
				if (!TIDISTASK(tids[i])) {
					cc = PvmBadParam;
					break;
				}

	}

	if (!cc && count > 0)
		cc = pvmmcast(pvmsbuf->m_mid, tids, count, tag);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_MCAST,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_mcast", cc);
	return cc;
}
