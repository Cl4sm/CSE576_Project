pvm_mytid()
{
	int cc;
	char **ep=0;

	TEV_DECLS;



	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_MYTID,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (!(cc = BEATASK))
		cc = pvmmytid;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_MYTID,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MT, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_mytid", cc);
	return cc;
}
