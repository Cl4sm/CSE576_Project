void
tev_init_pvmd( dst, ctx, tag )
	int dst;
	int ctx;
	int tag;
{
	struct Pvmtevdid *ptr;

	struct pmsg *mp;

	int i;

	/* Check for Existing Tracer */

	for ( i=init_start ; i != init_end ; i = (i + 1) % CHECKLISTSIZE )
	{
		if ( init_checklist[i] == dst )
			return;
	}

	init_checklist[init_end] = dst;

	init_end = ( init_end + 1 ) % CHECKLISTSIZE;

	init_start = ( init_end == init_start ) ?
		( init_start + 1 ) % CHECKLISTSIZE : init_start;

	/* Send Semantic Identifiers to Trace Collector */

	mp = mesg_new( 0 );

	mp->m_ctx = ctx;
	mp->m_tag = tag;
	mp->m_dst = dst;

	pkint( mp, TEV_MARK_DATA_ID );

	for ( i=0 ; i <= TEV_DID_MAX ; i++ )
	{
		ptr = &(pvmtevdidlist[i]);

		/* Pack Semantic ID, Name & Description */

		pkint( mp, i );

		pkstr( mp, ptr->did );

		pkstr( mp, ptr->desc );
	}

	pkint( mp, TEV_MARK_DATA_ID_END );

	sendmessage( mp );
}
