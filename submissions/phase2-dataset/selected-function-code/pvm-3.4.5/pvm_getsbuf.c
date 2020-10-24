pvm_getsbuf()
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETSBUF,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	cc = pvmsbuf ? pvmsbuf->m_mid : 0;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETSBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MSB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return cc;
}
