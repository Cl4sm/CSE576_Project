int
order_chunk( ostruct, ibuf, inchunk )
CHUNK_ORDER_PTR ostruct;
int ibuf;
CHUNK_PTR inchunk;  
{
	int *obseq;		/* order buffer sequence number */

	CHUNK_PTR firstchunk, tstchunk;	
	obseq = ostruct->bufseq + ibuf;

	inchunk->CHUNK_SEQ = *obseq; 


	/* store next packet sequence # to arrive in this buffer 
		from this sender*/
	*obseq = NEXTCHUNKSEQ(*obseq, ostruct->nbufs);

	if (ostruct -> oochunks == (CHUNK_PTR) NULL)
	{
		ostruct -> oochunks = inchunk;
		inchunk -> CHUNK_NEXT = inchunk -> CHUNK_PREV = inchunk;
	}
	else
	{
		firstchunk = tstchunk = ostruct->oochunks;
		while (SEQLESSTHAN(tstchunk->CHUNK_SEQ, inchunk -> CHUNK_SEQ)
				&& tstchunk -> CHUNK_NEXT != firstchunk)
			tstchunk = tstchunk -> CHUNK_NEXT;

		if (SEQLESSTHAN(tstchunk ->CHUNK_SEQ, inchunk->CHUNK_SEQ))
		{
			LISTPUTAFTER(tstchunk, inchunk, CHUNK_NEXT, CHUNK_PREV);
		}
		else
		{
			LISTPUTBEFORE(tstchunk, inchunk, CHUNK_NEXT, CHUNK_PREV);
			if (tstchunk == firstchunk) /* new first of the list ? */
				ostruct -> oochunks = inchunk;
		}
	}

	if (pvmdebmask & (PDMPACKET | PDMNODE))
	{
		sprintf(mpperrtext,"Ordering packet seq %d from tid %x\n", 
				inchunk -> CHUNK_SEQ, inchunk -> CHUNK_SRC); 
		pvmlogerror(mpperrtext);
	}
	return 0;
}	
