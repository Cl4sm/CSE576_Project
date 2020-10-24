enc_trc_ulong(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_ULONG | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_ULONG | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
#if	defined(I860_NODE)
		/* XXX are handwritten anyhow */
		return enc_xdr_long(mp, vp, cnt, std, siz);
#else
		return enc_xdr_ulong(mp, vp, cnt, std, siz);
#endif
	}
	return 0;
}
