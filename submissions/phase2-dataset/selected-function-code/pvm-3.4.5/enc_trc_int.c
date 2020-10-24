static int
enc_trc_int(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_INT | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_INT | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_int(mp, vp, cnt, std, siz);
	}

	/* Weasel Hack to Sneak in Trace Markers, with cnt == 0...  :-)   */
	/* Need to be able to insert markers without other data type and  */
	/* array size crapola - it's either this or a new pvm_pkmarker()  */
	/* routine and an enc_mrk for each encoding (which is more bogus).*/
	else if ( vp != NULL && *((int *) vp) < 0 ) {
		return enc_xdr_int(mp, vp, 1, 1, (int) sizeof(int));
	}

	return 0;
}
