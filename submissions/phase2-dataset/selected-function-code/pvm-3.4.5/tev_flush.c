tev_flush( setflag )
int setflag;
{
	char *event_names[ TEV_MAX - TEV_FIRST + 1 ];

	int timings_usec[ TEV_MAX - TEV_FIRST + 1 ];
	int timings_sec[ TEV_MAX - TEV_FIRST + 1 ];
	int counts[ TEV_MAX - TEV_FIRST + 1 ];

	struct timeval timestamp;

	int tsec, tusec;
	int routetmp;
	int savebuf;
	int tmpbuf;
	int num;
	int tmp;
	int i;

	if ( pvmtrc.trctid <= 0 || pvmmytid == pvmtrc.trctid )
		return;

	switch ( pvmtrc.trcopt )
	{
		case PvmTraceFull:
		{
			if ( setflag )
			{
				/* Verify Trace Record Message Buffer */

				if ( !pvmtrcsbf )
					return;

				/* Set Send Buffer */

				savebuf = pvm_setsbuf( pvmtrcsbf );
			}

			/* Pack End of Event Buffer Marker */
			/* (pvmtrcbuf == 0 means no buffering) */

			if ( pvmtrc.trcbuf )
			{
				tmp = TEV_MARK_EVENT_BUFFER_END;
				pvm_pkint( &tmp, 1, 1 );
			}

			break;
		}

		case PvmTraceTime:
		{
			/* Set Up Send Buffer */

			pvmtrccodef = &pvmtrccodef_desc;

			pvmtrcsbf = pvm_mkbuf( PvmDataDefault );
			pvmtrcmp = midtobuf( pvmtrcsbf );

			savebuf = pvm_setsbuf( pvmtrcsbf );

			/* Pack Event Marker */

			tmp = TEV_MARK_EVENT_DESC;
			pvm_pkint( &tmp, 1, 1 );

			/* Pack Descriptor Event ID & Event Name */

			tmp = TEV_TIMING;
			pvm_pkint( &tmp, 1, 1 );

			pvm_pkstr( pvmtevinfo[ TEV_TIMING - TEV_FIRST ].name );

			/* Pack Event Header */

			gettimeofday( &timestamp, (struct timezone *) 0 );

			tsec = (int) timestamp.tv_sec;
			tusec = (int) timestamp.tv_usec;

			TEV_PACK_INT( TEV_DID_TS, TEV_DATA_SCALAR, &tsec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TU, TEV_DATA_SCALAR, &tusec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TID, TEV_DATA_SCALAR,
				&pvmmytid, 1, 1 );

			/* Collect Timing Array */

			num = 0;

			for ( i=TEV_FIRST ; i <= TEV_MAX ; i++ )
			{
				if ( pvmtevinfo[i].count > 0 )
				{
					event_names[num] = pvmtevinfo[i].name;

					timings_sec[num] = (int) pvmtevinfo[i].total.tv_sec;
					timings_usec[num] =
						(int) pvmtevinfo[i].total.tv_usec;

					counts[num] = pvmtevinfo[i].count;

					pvmtevinfo[i].total.tv_sec =
						pvmtevinfo[i].total.tv_usec = 0;

					pvmtevinfo[i].count = 0;

					num++;
				}
			}

			/* Pack Number of Events & Arrays */

			TEV_PACK_INT( TEV_DID_VCT, TEV_DATA_SCALAR,
				&num, 1, 1 );

			TEV_PACK_STRING( TEV_DID_VID, TEV_DATA_ARRAY,
				event_names, num, 1 );

			TEV_PACK_INT( TEV_DID_TGS, TEV_DATA_ARRAY,
				timings_sec, num, 1 );

			TEV_PACK_INT( TEV_DID_TGU, TEV_DATA_ARRAY,
				timings_usec, num, 1 );

			TEV_PACK_INT( TEV_DID_PRF, TEV_DATA_ARRAY,
				counts, num, 1 );

			/* Pack End of Descriptor Marker */

			tmp = TEV_MARK_EVENT_DESC_END;
			pvm_pkint( &tmp, 1, 1 );

			setflag = 1;

			break;
		}

		case PvmTraceCount:
		{
			/* Set Up Send Buffer */

			pvmtrccodef = &pvmtrccodef_desc;

			pvmtrcsbf = pvm_mkbuf( PvmDataDefault );
			pvmtrcmp = midtobuf( pvmtrcsbf );

			savebuf = pvm_setsbuf( pvmtrcsbf );

			/* Pack Event Marker */

			tmp = TEV_MARK_EVENT_DESC;
			pvm_pkint( &tmp, 1, 1 );

			/* Pack Descriptor Event ID & Event Name */

			tmp = TEV_PROFILING;
			pvm_pkint( &tmp, 1, 1 );

			pvm_pkstr( pvmtevinfo[ TEV_PROFILING - TEV_FIRST ].name );

			/* Pack Event Header */

			gettimeofday( &timestamp, (struct timezone *) 0 );

			tsec = (int) timestamp.tv_sec;
			tusec = (int) timestamp.tv_usec;

			TEV_PACK_INT( TEV_DID_TS, TEV_DATA_SCALAR, &tsec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TU, TEV_DATA_SCALAR, &tusec, 1, 1 );
			TEV_PACK_INT( TEV_DID_TID, TEV_DATA_SCALAR,
				&pvmmytid, 1, 1 );

			/* Collect Profiling Array */

			num = 0;

			for ( i=TEV_FIRST ; i <= TEV_MAX ; i++ )
			{
				if ( pvmtevinfo[i].count > 0 )
				{
					event_names[num] = pvmtevinfo[i].name;

					counts[num] = pvmtevinfo[i].count;

					pvmtevinfo[i].count = 0;

					num++;
				}
			}

			/* Pack Number of Events & Arrays */

			TEV_PACK_INT( TEV_DID_VCT, TEV_DATA_SCALAR,
				&num, 1, 1 );

			TEV_PACK_STRING( TEV_DID_VID, TEV_DATA_ARRAY,
				event_names, num, 1 );

			TEV_PACK_INT( TEV_DID_PRF, TEV_DATA_ARRAY,
				counts, num, 1 );

			/* Pack End of Descriptor Marker */

			tmp = TEV_MARK_EVENT_DESC_END;
			pvm_pkint( &tmp, 1, 1 );

			setflag = 1;

			break;
		}

		default:
			pvmlogprintf(
				"Uh-Oh! Bogus Tracing Option (%d) in tev_flush()...\n",
				pvmtrc.trcopt );
			return;
	}

	/* Reset Send Buffer (Done Packing Stuff) */

	if ( setflag )
		pvm_setsbuf( savebuf );

	/* Clear Out Trace Buffer Global Before Entering mroute()... */
	/* (save in tmpbuf for sending, needed for mhf_invoke event) */

	tmpbuf = pvmtrcsbf;

	pvmtrcsbf = 0;

	/* Send Message */

	if ( (routetmp = pvmrouteopt) == PvmRouteDirect )
		pvmrouteopt = PvmAllowDirect;

	pvmtrcmp->m_ctx = pvmtrc.trcctx;

	mroute( tmpbuf, pvmtrc.trctid, pvmtrc.trctag, &pvmtrcztv );

	pvmrouteopt = routetmp;

	/* Clean-up */

	pvm_freebuf( tmpbuf );
}
