pvm_trecv(tid, tag, tmout)
	int tid;				/* source tid to match */
	int tag;				/* message tag to match */
	struct timeval *tmout;	/* time to wait for match */
{
	struct pmsg *up;
	struct pmsg *bestup;
	int bestcc = 0;
	int cc;
	struct timeval tin, tnow;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_TRECV,TEV_EVENT_ENTRY)) {
			int ts, tu;
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			if (tmout) {
				ts = tmout->tv_sec;
				tu = tmout->tv_usec;
			}
			else
				ts = tu = -1;
			TEV_PACK_INT( TEV_DID_MTS, TEV_DATA_SCALAR, &ts, 1, 1 );
			TEV_PACK_INT( TEV_DID_MTU, TEV_DATA_SCALAR, &tu, 1, 1 );
			TEV_FIN;
		}
	}

	pvmgetclock(&tin);

	if (!(cc = BEATASK)) {
		if (pvmrbuf)
			umbuf_free(pvmrbuf);
		pvmrbuf = 0;

		for (up = pvmrxlist->m_link; 1; up = up->m_link) {
			if (up == pvmrxlist && bestcc)
				break;
			while (up == pvmrxlist) {
				up = up->m_rlink;
				if (tmout) {
					pvmgetclock(&tnow);
					TVXSUBY(&tnow, &tnow, &tin);
					if (TVXLTY(tmout, &tnow)) {
						if (bestcc)
							goto fnd;
						cc = 0;
						if (!TVISSET(tmout)) {
							if ((cc = mroute(0, 0, 0, tmout)) <= 0)
								goto done;
						} else
							goto done;
					} else {
						TVXSUBY(&tnow, tmout, &tnow);
						if ((cc = mroute(0, 0, 0, &tnow)) < 0)
							goto done;
					}

				} else {
					if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
						goto done;
				}
				up = up->m_link;
			}

			if ((cc = recv_match(up->m_mid, tid, tag)) < 0)
				goto done;
			if (cc == 1) {
				bestup = up;
				break;
			}
			if (cc > bestcc) {
				bestcc = cc;
				bestup = up;
			}
		}

fnd:
		LISTDELETE(bestup, m_link, m_rlink);
		bestup->m_flag &= ~MM_UPACK;
		if (!(cc = pvm_setrbuf(bestup->m_mid)))
			cc = bestup->m_mid;
	}

done:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_TRECV,TEV_EVENT_EXIT)) {
			int nb, mc, src;
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (cc > 0)
				pvm_bufinfo(cc, &nb, &mc, &src);
			else
				nb = mc = src = -1;
			TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR, &nb, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &mc, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_PACK_INT( TEV_DID_SRC, TEV_DATA_SCALAR, &src, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_trecv", cc);
	return cc;
}
