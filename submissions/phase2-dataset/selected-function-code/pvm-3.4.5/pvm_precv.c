pvm_precv(tid, tag, cp, len, dt, rtid, rtag, rlen)
	int tid;
	int tag;
	void *cp;
	int len;
	int dt;
	int *rtid;
	int *rtag;
	int *rlen;
{
	static int last_rbf = 0;

	int nb, mc, src;
	int rbf;
	int cc = 0;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PRECV,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
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
		if (last_rbf > 0) {
			pvm_freebuf(last_rbf);
			last_rbf = 0;
		}
		rbf = pvm_setrbuf(0);
		cc = pvm_recv(tid, tag);
		if (cc > 0) {
			pvm_bufinfo(cc, &nb, &mc, &src);
			if (rlen)
				*rlen = nb;
			if (nb < len)
				len = nb;
			if (rtag)
				*rtag = mc;
			if (rtid)
				*rtid = src;
			pvm_upkbyte((char *)cp, len, 1);
			last_rbf = cc;
			cc = 0;
		}
		pvm_setrbuf(rbf);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PRECV,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			if ( cc < 0 )
				nb = mc = src = -1;
			TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR, &nb, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &mc, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_PACK_INT( TEV_DID_SRC, TEV_DATA_SCALAR, &src, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_precv", cc);
	return cc;
}
