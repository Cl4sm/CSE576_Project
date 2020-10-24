int
pvm_tc_settrace(mid)
	int mid;
{
	int trctid;
	int trcctx;
	int trctag;
	int outtid;
	int outctx;
	int outtag;
	char buf[256];
	int tbuf;
	int topt;

	pvm_upkint(&trctid, 1, 1);
	pvm_upkint(&trcctx, 1, 1);
	pvm_upkint(&trctag, 1, 1);
	pvm_upkint(&outtid, 1, 1);
	pvm_upkint(&outctx, 1, 1);
	pvm_upkint(&outtag, 1, 1);
	pvm_upkstr(buf);
	pvm_upkint(&tbuf, 1, 1);
	pvm_upkint(&topt, 1, 1);

	if (trctid) {
		/* cheat on trcctx & trctag to avoid race */
		pvmtrc.trcctx = trcctx;
		pvmtrc.trctag = trctag;
		pvm_setopt( PvmSelfTraceTid, trctid );
		if (strlen(buf) + 1 == TEV_MASK_LENGTH)
			BCOPY(buf, pvmtrc.tmask, TEV_MASK_LENGTH);
		else {
			TEV_MASK_INIT(pvmtrc.tmask);
			pvmlogerror("pvm_tc_settrace() bogus trace mask\n");
		}
		BCOPY(pvmtrc.tmask, pvmctrc.tmask, TEV_MASK_LENGTH);
		if (tbuf >= 0)
			pvmtrc.trcbuf = tbuf;
		else {
			pvmtrc.trcbuf = 0;
			pvmlogerror("pvm_tc_settrace() bogus trace buffering\n");
		}
		if (topt >= 0)
			pvmtrc.trcopt = topt;
		else {
			pvmtrc.trcopt = 0;
			pvmlogerror("pvm_tc_settrace() bogus trace options\n");
		}
	}

	if (outtid) {
		/* cheat on outctx & outtag to avoid race */
		pvmtrc.outctx = outctx;
		pvmtrc.outtag = outtag;
		pvm_setopt( PvmSelfOutputTid, outtid );
	}

	pvm_freebuf(mid);
	return 0;
}
