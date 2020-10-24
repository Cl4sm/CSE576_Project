int
pvm_upkstr(cp)
	char *cp;
{
	int l;
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKSTR,TEV_EVENT_ENTRY)) {
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_FIN;
		}
	}

	if (!pvmrbuf)
		cc = PvmNoBuf;
	else {
		if (!(cc = (pvmrbuf->m_codef->dec_int)
				(pvmrbuf, (void*)&l, 1, 1, sizeof(int))))
			cc = (pvmrbuf->m_codef->dec_byte)
					(pvmrbuf, (void*)cp, l, 1, 1);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKSTR,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkstr", cc) : PvmOk);
}
