pvm_setcontext(newctx)
	int newctx;
{
	int c;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SETCONTEXT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_CXS, TEV_DATA_SCALAR, &newctx, 1, 1 );
			TEV_FIN;
		}
	}

	c = pvmmyctx;
	pvmmyctx = newctx;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SETCONTEXT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &c, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return c;
}
