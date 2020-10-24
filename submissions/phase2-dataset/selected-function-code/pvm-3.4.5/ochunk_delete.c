CHUNK_PTR
ochunk_delete( ostruct )
CHUNK_ORDER_PTR ostruct;
{
	CHUNK_PTR tstchunk;

	if ( !ostruct || ! (ostruct->oochunks))
		return (CHUNK_PTR) NULL;

	tstchunk = ostruct->oochunks;

	if (tstchunk -> CHUNK_SEQ == ostruct -> seq)  /* yep, we got one */
	{
		ostruct->seq = NEXTCHUNKSEQ(ostruct -> seq, 1);
		if (tstchunk -> CHUNK_NEXT == tstchunk)
			ostruct -> oochunks = (CHUNK_PTR) NULL;
		else
		{
			ostruct -> oochunks = tstchunk -> CHUNK_NEXT;
			LISTDELETE(tstchunk, CHUNK_NEXT, CHUNK_PREV);
		}

		if (pvmdebmask & (PDMPACKET | PDMNODE))
		{
			sprintf(mpperrtext,"ochunk_delete() packet seq %d from tid %x\n", 
					tstchunk -> CHUNK_SEQ, tstchunk -> CHUNK_SRC); 
			pvmlogerror(mpperrtext);
		}

		tstchunk -> CHUNK_NEXT = tstchunk -> CHUNK_PREV = (CHUNK_PTR) NULL;
		tstchunk -> CHUNK_SEQ = 0;
		
		return tstchunk;
	}	

	return (CHUNK_PTR) NULL;
}
