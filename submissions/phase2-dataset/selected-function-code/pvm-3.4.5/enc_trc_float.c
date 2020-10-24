static int
enc_trc_float(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_FLOAT | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_FLOAT | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_float(mp, vp, cnt, std, siz);
	}
	return 0;
}
