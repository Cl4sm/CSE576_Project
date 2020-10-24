int
pvm_probe(tid, tag)
	int tid;
	int tag;
{
	struct pmsg *up;
	struct pmsg *bestup;
	int bestcc = 0;
	int cc;
	int alrdy = 0;
	TEV_DECLS
	static struct timeval ztv = { 0, 0 };

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PROBE,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		for (up = pvmrxlist->m_link; 1; up = up->m_link) {
			if (up == pvmrxlist && bestcc)
				break;
			while (up == pvmrxlist) {
				if (alrdy) {
					cc = 0;
					goto done;
				}
				up = up->m_rlink;
				if ((cc = mroute(0, 0, 0, &ztv)) < 0)
					goto done;
				up = up->m_link;
				alrdy = 1;
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
		bestup->m_flag &= ~MM_UPACK;
		cc = bestup->m_mid;
	}

done:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PROBE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_probe", cc);
	return cc;
}
