int
pvm_mkbuf(enc)
	int enc;		/* data format */
{
	struct pmsg *mp;
	struct frag *fp;
	int dotrchdr = 0;
	int cc = 0;
	int tmp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_MKBUF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_ME, TEV_DATA_SCALAR, &enc, 1, 1 );
			TEV_FIN;
		}
	}

	switch (enc) {								/* verify encoding */
	case PvmDataDefault:
		enc = 0x10000000;		/* XXX this just forces XDR */
		break;

	case PvmDataRaw:
		enc = pvmmydsig;
		break;

	case PvmDataInPlace:
		enc = 0x20000000;
		break;

	case PvmDataTrace:
		enc = 0x40000000;
		dotrchdr++;
		break;

	default:
		cc = PvmBadParam;
	}

	if (!cc) {
		if (mp = umbuf_new()) {
			mp->m_src = pvmmytid;
			pmsg_setenc(mp, enc);
			cc = mp->m_mid;
			if (dotrchdr) {
				tmp = enc_trc_hdr(mp);
				cc = ( tmp < 0 ) ? tmp : cc;
			}
		} else
			cc = PvmNoMem;
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_MKBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_mkbuf", cc);
	return cc;
}
