int
pvm_halt()
{
	int cc, sbf, rbf;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_HALT,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		cc = (msendrecv(TIDPVMD, TM_HALT, SYSCTX_TM) < 0) ? 0 : PvmSysErr;
		pvm_freebuf(pvm_setsbuf(sbf));
		pvm_setrbuf(rbf);
	}

	if (TEV_AMEXCL) {
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_halt", cc);
	return cc;
}
