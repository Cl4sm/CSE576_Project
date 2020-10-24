int
pvm_kill(tid)
	int tid;
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_KILL,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TT, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_FIN;
		}
	}

	cc = pvm_sendsig(tid, SIGTERM);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_KILL,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	if (cc < 0)
		lpvmerr("pvm_kill", cc);
	return cc;
}
