int
pvm_pkstr(cp)
	char *cp;
{
	int l = strlen(cp) + 1;
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKSTR,TEV_EVENT_ENTRY)) {
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_FIN;
		}
	}

	if (!pvmsbuf)
		cc = PvmNoBuf;
	else {
		/* can't do InPlace without a lot of grief */
		if (pvmsbuf->m_enc == 0x20000000)
			cc = PvmNotImpl;
		/* user defined, don't pack len, don't pack '\0' */
		else if (pvmsbuf->m_enc == 0x40000000)
			cc = (pvmsbuf->m_codef->enc_byte)
				(pvmsbuf, (void*)cp, l - 1, 1, 1);
		else
			if (!(cc = (pvmsbuf->m_codef->enc_int)
					(pvmsbuf, (void*)&l, 1, 1, sizeof(int))))
				cc = (pvmsbuf->m_codef->enc_byte)
						(pvmsbuf, (void*)cp, l, 1, 1);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKSTR,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkstr", cc) : PvmOk);
}
