CHUNK_PTR
pvm_chunkReady(ppmsg, nbufs, mfunc, ostructs, nstructs, cbuf, hdReadyList )
MSG_INFO_PTR ppmsg;	/* vector of pre-posted message buffers */
int nbufs;				/* number of bufs in ppsmg */
MSGFUNC_PTR mfunc;		/* structure of message passing primitives */
MPP_DIRECTI_PTR	ostructs;
int nstructs;
int *cbuf;				/* buffer to probe in ppmsg vector */
CHUNK_PTR *hdReadyList;

{
	char *cp;
	char errtxt[64];

	MPP_DIRECTI_PTR tcon;

	int cc;
	int i, ipctxt, ipff, iplen, ipsrc, iptag;
	int flen;
	int len;
	int match;
	int src;
	int tag, tpff, tpsrc; 
	
	CHUNK *rcp, *rcp2; 

	struct msgid *precvMsg, *precvIds;

	if (*hdReadyList)
		if ((*hdReadyList)->CHUNK_NEXT != *hdReadyList)
		{
 			rcp = (*hdReadyList)->CHUNK_NEXT;
			LISTDELETE(rcp, CHUNK_NEXT, CHUNK_PREV);
			return rcp;
		}

	/*  check if there is a ready chunk in the preposted buffers.  
		repost the receive for the next time around */ 

	for (i = 0; i < nbufs; i ++)
	{
		if ( *cbuf >= nbufs || *cbuf < 0) 
			*cbuf = 0;

		if (rcp = read_chunk(ppmsg + (*cbuf), &src, &tag, &len, MPPREPOST, 
				MPPANY, mfunc)) 	
		{
			rcp->CHUNK_LEN = len; 	/* remember how much data was sent */

			rcp->CHUNK_SRC = src; 	/* this will be a node #, not a tid ! */

			if (pvmdebmask & (PDMNODE | PDMPACKET))
			{
				tpff = pvmget8(rcp->CHUNK_DAT + 12);

				tpsrc = pvmget32(rcp->CHUNK_DAT + 4);

				sprintf(errtxt, "readyChunk()read_chunk src %x node %d len %d ff %d buffer %d\n",
						tpsrc, src, len, tpff, *cbuf);
				pvmlogerror(errtxt);

			}


			/* find the ordering structure for this task, put this chunk
				on this tasks reorder queue */

			if ( !(tcon =  pvm_find_direct(ostructs, nstructs, src)))
			{
				pvmlogerror("frReady() could not find ordering struct \n");	
				return (CHUNK_PTR) NULL;
			}
			else
				order_chunk(tcon->ordering, *cbuf, rcp); /* put on order q */
		
			/*  remove in-order chunks on for this task, put them in 
				callers readyFrags queue */

			while ( rcp = ochunk_delete(tcon->ordering)) /* de-queue chunks */
			{
				if (!*hdReadyList)   /* readyList is empty, initialize */
					*hdReadyList = CHUNK_NEW(0);


				if (pvmdebmask & (PDMNODE | PDMPACKET))
					pvmlogprintf("dequeing chunk from %x len %d\n", rcp->CHUNK_SRC, rcp->CHUNK_LEN);	
				/* XXX - This next part is really ugly! It checks if this
			     *	chunk is an inplace header and if there is a 
			     *	precv outstanding for it. If there is, then the message
			 	 *	can go directly into the user memory. 
				 *
				 * for daemons, it is sufficient to set precvIds to NULL
				 * This has to be done *AFTER* the frag header has been
				 * dequeued from the readyList to preserve ordering
				*/
	
				if ( (flen = pvmget32(rcp->CHUNK_DAT + 8 ) + TDFRAGHDR)  
						!= rcp->CHUNK_LEN )
				{
					ipff = pvmget8(rcp->CHUNK_DAT + 12);
	
					match = 0;
	
					if (ipff  & FFSOM ) /* start of message, check for precv*/ 
					{
						ipsrc = pvmget32(rcp->CHUNK_DAT + 4);
	
						iptag = pvmget32(rcp->CHUNK_DAT + TDFRAGHDR + 4);
	
						ipctxt = pvmget32(rcp->CHUNK_DAT + TDFRAGHDR + 8);
	
				 		if 	(precvMsg = pvm_mpp_get_precvids()) 
						{
							if ( match = (((precvMsg->otid == -1 || ipsrc == precvMsg->otid)
									&& (precvMsg->tag  == -1 || iptag == precvMsg->tag)
									&& (ipctxt == precvMsg->ctxt)) ? 1 : 0) )
							{
								cc = inplaceRecv( src, src, precvMsg->ubuf, 
									precvMsg->len, &iplen, mfunc);	
								precvMsg->len = iplen;
								precvMsg->complete = 1; /* got it ! */
								rcp->CHUNK_RIP = 1; 	/* received in place */
							}
						}
					}
	
					if (!match)
					{
						if ( flen > rcp->CHUNK_MAX ) /* won't fit */
						{
							rcp2 = CHUNK_NEW(flen);
	
							BCOPY(rcp->CHUNK_DAT, rcp2->CHUNK_DAT, rcp->CHUNK_LEN);
	
							rcp2->CHUNK_SRC = rcp->CHUNK_SRC;
	
							rcp2->CHUNK_LEN = rcp->CHUNK_LEN;
	
							CHUNK_FREE(rcp);
	
							rcp = rcp2;
			
						}
	
						cc = inplaceRecv(src, src, rcp->CHUNK_DAT + rcp->CHUNK_LEN,
							rcp->CHUNK_MAX - rcp->CHUNK_LEN, &iplen, mfunc);
	
						rcp->CHUNK_LEN += iplen;
	
					}
						 
				}

				LISTPUTBEFORE(*hdReadyList, rcp, CHUNK_NEXT, CHUNK_PREV);
			}

			(*cbuf) ++;		/* increment to probe the next buffer */
			break;
		}

		(*cbuf) ++;		/* increment to probe the next buffer */
		
	}


	/* if ordered frags have been put into the global ready list, then
		dequeue one of them */

	if (*hdReadyList && (*hdReadyList)->CHUNK_NEXT != *hdReadyList)
	{
 		rcp = (*hdReadyList)->CHUNK_NEXT;
		LISTDELETE(rcp, CHUNK_NEXT, CHUNK_PREV);		
		if (pvmdebmask & (PDMNODE | PDMPACKET))
			pvmlogprintf(" chunk  from hdready from %x len %d\n", rcp->CHUNK_SRC, rcp->CHUNK_LEN);	
		return rcp;
	}
	else
		return (CHUNK_PTR) NULL;

}
