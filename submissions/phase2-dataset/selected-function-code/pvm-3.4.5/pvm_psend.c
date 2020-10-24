int
pvm_psend(tid, tag, cp, len, dt)
	int tid;
	int tag;
	void *cp;
	int len;
	int dt;
{
	int sbf;
	int cc = 0;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PSEND,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_DST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &len, 1, 1 );
			TEV_PACK_INT( TEV_DID_PDT, TEV_DATA_SCALAR, &dt, 1, 1 );
			TEV_FIN;
		}
	}

	switch (dt) {

	case PVM_BYTE:
		len *= sizeof(char);
		break;

	case PVM_SHORT:
	case PVM_USHORT:
		len *= sizeof(short);
		break;

	case PVM_INT:
	case PVM_UINT:
		len *= sizeof(int);
		break;

	case PVM_LONG:
	case PVM_ULONG:
		len *= sizeof(long);
		break;

	case PVM_FLOAT:
		len *= sizeof(float);
		break;

	case PVM_CPLX:
		len *= sizeof(float) * 2;
		break;

	case PVM_DOUBLE:
		len *= sizeof(double);
		break;

	case PVM_DCPLX:
		len *= sizeof(double) * 2;
		break;

	case PVM_STR:
		cc = PvmNotImpl;
		break;

	default:
		cc = PvmBadParam;
		break;
	}

	if (!cc) {
#if 0
		if ((tid & pvmtidhmask) == (pvmmytid & pvmtidhmask)
		&& (tid & ~pvmtidhmask) != TIDPVMD) {		/* to local task */
			char *dbuf;

			len += sizeof(struct shmpghdr);
			if ((dbuf = dynbuf(tid, len)) != (char *)-1L) {
				BCOPY(cp, dbuf, len);

			} else
				cc = PvmSysErr;

		} else
#endif /*0*/
		{
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataRaw));
			pvm_pkbyte((char *)cp, len, 1);
			if ((cc = pvm_send(tid, tag)) > 0)
				cc = 0;
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PSEND,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_psend", cc);
	return cc;
}
