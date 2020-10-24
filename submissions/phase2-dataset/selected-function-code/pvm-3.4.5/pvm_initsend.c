int
pvm_initsend(enc)
	int enc;
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_INITSEND,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_ME, TEV_DATA_SCALAR, &enc, 1, 1 );
			TEV_FIN;
		}
	}

	if ((cc = pvm_mkbuf(enc)) >= 0) {
		if (pvmsbuf)
			pvm_freebuf(pvmsbuf->m_mid);
		pvm_setsbuf(cc);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_INITSEND,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MSB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_initsend", cc);
	return cc;
}
