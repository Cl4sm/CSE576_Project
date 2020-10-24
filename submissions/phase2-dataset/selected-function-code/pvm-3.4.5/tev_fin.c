int
tev_fin()
{
	struct timeval timestamp;

	int flush;
	int size;
	int tmp;

	switch ( pvmtrc.trcopt )
	{
		case PvmTraceFull:
		{
			/* Pack End of Event Descriptor Marker */

			if ( pvmtrcdesc )
			{
				tmp = TEV_MARK_EVENT_DESC_END;
				pvm_pkint( &tmp, 1, 1 );
			}

			/* Pack End of Event Marker */

			else
			{
				tmp = TEV_MARK_EVENT_RECORD_END;
				pvm_pkint( &tmp, 1, 1 );
			}

			/* Reset Send Buffer (Done Packing Stuff) */

			pvm_setsbuf( pvmtrcsbfsave );
			pvmtrcsbfsave = 0;

			/* Check for Trace Message Send */
			/* (pvmtrc.trcbuf == 0 means no buffering) */

			flush = 0;

			if ( !pvmtrc.trcbuf )
				flush++;

			else
			{
				if ( pvm_bufinfo( pvmtrcsbf, &size,
						(int *) NULL, (int *) NULL ) ) {
					pvmlogerror(
						"tev_fin() error - get trace buffer size\n" );
					size = -1;
				}

				if ( size >= pvmtrc.trcbuf )
					flush++;
			}

			if ( flush )
				tev_flush( 1 );
	
			break;
		}

		case PvmTraceTime:
			gettimeofday( &timestamp, (struct timezone *) 0 );
			TVXSUBY( &(pvmtevinfo[ pvmtrcsavekind - TEV_FIRST ].total),
				&timestamp,
				&(pvmtevinfo[ pvmtrcsavekind - TEV_FIRST ].mark) );
			(pvmtevinfo[ pvmtrcsavekind - TEV_FIRST ].count)++;
			break;

		case PvmTraceCount:
			(pvmtevinfo[ pvmtrcsavekind - TEV_FIRST ].count)++;
			break;

		default:
			pvmlogprintf(
				"Uh-Oh! Bogus Tracing Option (%d) in tev_fin()...\n",
				pvmtrc.trcopt );
			break;
	}

	return( 1 );
}
