pvm_spawn(file, argv, flags, where, count, tids)
	char *file;
	char **argv;
	int flags;
	char *where;
	int count;
	int *tids;
{
	int sbf, rbf;	/* temp for current tx, rx msgs */
	int ictx;
	int cc;
	int i, n;
	char **ep;
	int *tidlist = 0;
	char *p;
	char buf[TEV_MASK_LENGTH + 20];
	TEV_DECLS

	if (p = getenv("PVMTASK"))
		i = pvmstrtoi(p) | flags;
	else
		i = flags;

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SPAWN,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_SE, TEV_DATA_SCALAR,
				file ? file : "", 1, 1 );
			TEV_PACK_STRING( TEV_DID_SW, TEV_DATA_SCALAR,
				where ? where : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_SF, TEV_DATA_SCALAR, &i, 1, 1 );
			TEV_PACK_INT( TEV_DID_SC, TEV_DATA_SCALAR, &count, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (count < 1)
			cc = PvmBadParam;

		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);

			pvm_pkstr(file);
			pvm_pkint(&i, 1, 1);
			pvm_pkstr(where ? where : "");
			pvm_pkint(&count, 1, 1);
			if (argv)
				for (n = 0; argv[n]; n++);
			else
				n = 0;
			pvm_pkint(&n, 1, 1);
			for (i = 0; i < n; i++)
				pvm_pkstr(argv[i]);

			pvm_pkint( &(pvmctrc.outtid), 1, 1 );
			pvm_pkint( &(pvmctrc.outctx), 1, 1 );
			pvm_pkint( &(pvmctrc.outtag), 1, 1 );
			pvm_pkint( &(pvmctrc.trctid), 1, 1 );
			pvm_pkint( &(pvmctrc.trcctx), 1, 1 );
			pvm_pkint( &(pvmctrc.trctag), 1, 1 );

			n = pvmgetenvars(&ep) + 4;
			pvm_pkint(&n, 1, 1);
			n -= 4;
			sprintf(buf, "PVMTMASK=%s", pvmctrc.tmask);
			pvm_pkstr(buf);
			sprintf(buf, "PVMTRCBUF=%d", pvmctrc.trcbuf);
			pvm_pkstr(buf);
			sprintf(buf, "PVMTRCOPT=%d", pvmctrc.trcopt);
			pvm_pkstr(buf);
			sprintf(buf, "PVMCTX=0x%x", pvmmyctx);
			pvm_pkstr(buf);
			if (n > 0) {
				for (i = 0; i < n; i++)
					pvm_pkstr(ep[i]);
				PVM_FREE(ep);
			}

			if (pvmschedtid)
				cc = msendrecv(pvmschedtid, SM_SPAWN, PvmBaseContext);
			else
				cc = msendrecv(TIDPVMD, TM_SPAWN, SYSCTX_TM);
			if (cc > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc == count) {
					tidlist = tids ? tids : TALLOC(count, int, "xxx");
					pvm_upkint(tidlist, cc, 1);
					cc = bubble(cc, tidlist);
				}
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));

			if (cc > 0) {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				pvm_pkint(&cc, 1, 1);
				pvm_pkint(tidlist, cc, 1);
				ictx = pvm_setcontext(SYSCTX_TC);
				pvmmcast(pvmsbuf->m_mid, tidlist, cc, TC_SIBLINGS);
				pvm_setcontext(ictx);
				pvm_freebuf(pvm_setsbuf(sbf));
			}
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SPAWN,TEV_EVENT_EXIT)) {
			int tevcount;
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			tevcount = ( cc > 0 ) ? cc : 0;
			TEV_PACK_INT( TEV_DID_STL, TEV_DATA_ARRAY,
				tidlist, tevcount, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (tidlist != tids && tidlist != NULL)
		PVM_FREE(tidlist);

	if (cc < 0)
		lpvmerr("pvm_spawn", cc);
	return cc;
}
