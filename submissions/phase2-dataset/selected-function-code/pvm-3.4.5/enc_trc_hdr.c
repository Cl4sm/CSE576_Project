int /* not static */
enc_trc_hdr(mp)
	struct pmsg *mp;
{
	struct timeval timestamp;

	int tsec, tusec;
	int tmp;
	int cc;

	gettimeofday( &timestamp, (struct timezone *) 0 );

	tsec = (int) timestamp.tv_sec;
	tusec = (int) timestamp.tv_usec;

	if ( (cc = enc_xdr_init(mp)) )
		return( cc );

	tmp = TEV_MARK_USER_EVENT_RECORD;
	if ((cc = enc_xdr_int(mp, (void *) &tmp, 1, 1, (int) sizeof(int))))
		return( cc );

	tmp = TEV_USER_DEFINED;
	if ((cc = enc_xdr_int(mp, (void *) &tmp, 1, 1, (int) sizeof(int))))
		return( cc );

	tmp = strlen( pvmtevinfo[TEV_USER_DEFINED].name ) + 1;
	if ((cc = enc_xdr_int(mp, (void *) &tmp, 1, 1, (int) sizeof(int))))
		return( cc );

	if ((cc = enc_xdr_byte(mp,
			(void *)(pvmtevinfo[TEV_USER_DEFINED].name), tmp, 1, 1)))
		return( cc );

	if ((cc = enc_xdr_int(mp, (void *) &tsec, 1, 1,
			(int) sizeof(int))))
		return( cc );
	if ((cc = enc_xdr_int(mp, (void *) &tusec, 1, 1,
			(int) sizeof(int))))
		return( cc );

	return( 0 );
}
