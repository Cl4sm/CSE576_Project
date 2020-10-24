tev_begin( kind, entry_exit )
int kind;
int entry_exit;
{
	struct timeval timestamp;

	int tsec, tusec;
	int newbuffer;
	int tmp;

	/* Get Timestamp */

	if ( pvmtrc.trcopt != PvmTraceCount )
	{
		gettimeofday( &timestamp, (struct timezone *) 0 );

		tsec = (int) timestamp.tv_sec;
		tusec = (int) timestamp.tv_usec;
	}

	switch ( pvmtrc.trcopt )
	{
		case PvmTraceFull:
		{
			/* Check for Descriptor Dump (one each, begin/end events) */

			if ( !( (pvmtevinfo[ kind - TEV_FIRST ].desc_status)
				& entry_exit ) )
			{
				pvmtevinfo[ kind - TEV_FIRST ].desc_status |=
					entry_exit;

				/* Set Trace Event Packing Vector */

				pvmtrccodef = &pvmtrccodef_desc;

				/* Set Descriptor Mode */

				pvmtrcdesc = 1;
			}

			/* Set Regular Trace Event Packing & Mode */

			else
			{
				pvmtrccodef = &pvmtrccodef_raw;
			
				pvmtrcdesc = 0;
			}

			/* Initialize Trace Event Message Buffer? */

			newbuffer = 0;

			if ( !pvmtrcsbf )
			{
				pvmtrcsbf = pvm_mkbuf( PvmDataDefault );
				pvmtrcmp = midtobuf( pvmtrcsbf );

				newbuffer++;
			}

			/* Select Trace Event Buffer (save current) */

			pvmtrcsbfsave = pvm_setsbuf( pvmtrcsbf );

			/* Pack Event Buffer Marker */

			if ( newbuffer && pvmtrc.trcbuf )
			{
				tmp = TEV_MARK_EVENT_BUFFER;
				pvm_pkint( &tmp, 1, 1 );
			}

			/* Descriptor Header */

			if ( pvmtrcdesc )
			{
				/* Pack Event Marker */

				tmp = TEV_MARK_EVENT_DESC;
				pvm_pkint( &tmp, 1, 1 );

				/* Pack Descriptor Event ID & Event Name */

				tmp = kind | entry_exit;
				pvm_pkint( &tmp, 1, 1 );

				pvm_pkstr( pvmtevinfo[ kind - TEV_FIRST ].name );
			}

			/* Regular Header */

			else
			{
				/* Pack Event Marker */

				tmp = TEV_MARK_EVENT_RECORD;
				pvm_pkint( &tmp, 1, 1 );

				/* Pack Event ID */

				tmp = kind | entry_exit;
				pvm_pkint( &tmp, 1, 1 );
			}

			/* Pack Event Header */

			TEV_PACK_INT( TEV_DID_TS, TEV_DATA_SCALAR, &tsec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TU, TEV_DATA_SCALAR, &tusec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TID, TEV_DATA_SCALAR,
				&pvmmytid, 1, 1 );
			
			break;
		}

		case PvmTraceTime:
			pvmtrccodef = &pvmtrccodef_nop;
			pvmtevinfo[ kind - TEV_FIRST ].mark = timestamp;
			pvmtrcsavekind = kind;
			break;

		case PvmTraceCount:
			pvmtrccodef = &pvmtrccodef_nop;
			pvmtrcsavekind = kind;
			break;

		default:
			pvmlogprintf(
				"Uh-Oh! Bogus Tracing Option (%d) in tev_begin()...\n",
				pvmtrc.trcopt );
			pvmtrccodef = &pvmtrccodef_nop;
			break;
	}

	return( 1 );
}
