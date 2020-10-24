tev_send_endtask( dst, ctx, tag, tid, status, usec, uusec, ssec, susec )
	int dst;
	int ctx;
	int tag;
	int tid;
	int status;
	long usec;
	long uusec;
	long ssec;
	long susec;
{
	static int checklist[CHECKLISTSIZE];
	static int start = 0;
	static int end = 0;

	struct timeval now;

	struct pmsg *mp;

	int flag;
	int tmp;
	int i;

	mp = mesg_new( 0 );

	mp->m_ctx = ctx;
	mp->m_tag = tag;
	mp->m_dst = dst;

	gettimeofday( &now, (struct timezone *) 0 );

	/* Check for Init */

	tev_init_pvmd( dst, ctx, tag );

	/* Check for Descriptor */

	flag = 0;

	for ( i=start ; i != end && !flag ; i = (i + 1) % CHECKLISTSIZE )
	{
		if ( checklist[i] == dst )
			flag++;
	}

	/* Pack ENDTASK Event Message (with descriptor if necessary) */

	/* Header */

	if ( !flag )
	{
		pkint( mp, TEV_MARK_EVENT_DESC );

		pkint( mp, TEV_ENDTASK );

		pkstr( mp, pvmtevinfo[ TEV_ENDTASK ].name );

		checklist[end] = dst;

		end = ( end + 1 ) % CHECKLISTSIZE;

		start = ( end == start ) ?
			( start + 1 ) % CHECKLISTSIZE : start;
	}

	else
	{
		pkint( mp, TEV_MARK_EVENT_RECORD );

		pkint( mp, TEV_ENDTASK );
	}

	/* Time Stamp Seconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TS );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) now.tv_sec );

	/* Time Stamp Microseconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TU );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) now.tv_usec );

	/* Task ID */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TID );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, tid );

	/* Task Status */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TST );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, status );

	/* User Time Seconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TUS );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) usec );

	/* User Time Microseconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TUU );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) uusec );

	/* System Time Seconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TSS );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) ssec );

	/* System Time Microseconds */

	if ( !flag )
	{
		pkint( mp, TEV_DID_TSU );
		pkint( mp, TEV_DATA_INT | TEV_DATA_SCALAR );
	}

	pkint( mp, (int) susec );

	/* End Marker */

	if ( !flag )
		pkint( mp, TEV_MARK_EVENT_DESC_END );

	else
		pkint( mp, TEV_MARK_EVENT_RECORD_END );

	sendmessage( mp );
}
