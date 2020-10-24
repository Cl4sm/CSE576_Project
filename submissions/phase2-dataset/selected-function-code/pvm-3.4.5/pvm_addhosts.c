int
pvm_addhosts(names, count, svp)
	char **names;	/* host name vector */
	int count;		/* length of names */
	int *svp;		/* status vector return */
{
	int sbf, rbf;
	int cc;
	int i;
	int *sv;		/* status vector */
	char *buf;
	int tmp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_ADDHOSTS,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_HNL, TEV_DATA_ARRAY,
				names, count, 1 );
			TEV_FIN;
		}
	}

	if (count < 1 || count > (TIDHOST >> (ffs(TIDHOST) - 1))) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkint(&count, 1, 1);
			for (i = 0; i < count; i++)
				pvm_pkstr(names[i]);

			if (pvmschedtid)
				cc = msendrecv(pvmschedtid, SM_ADDHOST, PvmBaseContext);
			else
				cc = msendrecv(TIDPVMD, TM_ADDHOST, SYSCTX_TM);
			if (cc > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc >= 0) {
					if (cc == count) {
						pvm_upkint(&cc, 1, 1);	/* toss narches */
						sv = TALLOC(count, int, "sv1");
						cc = 0;
						for (i = 0; i < count; i++) {
							pvm_upkint(&sv[i], 1, 1);
							/* toss name, arch, speed, ds */
							pvmupkstralloc(&buf);	PVM_FREE(buf);
							pvmupkstralloc(&buf);	PVM_FREE(buf);
							pvm_upkint(&tmp, 1, 1);
							pvm_upkint(&tmp, 1, 1);
							if (sv[i] >= 0)
								cc++;
						}
						if (svp)
							BCOPY((char*)sv, (char*)svp, count * sizeof(int));
						PVM_FREE(sv);

					} else {
						pvmlogprintf("pvm_addhosts() sent count %d received count %d\n",
								count, cc);
						cc = PvmOutOfRes;
					}
				}
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_ADDHOSTS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_addhosts", cc);
	return cc;
}
