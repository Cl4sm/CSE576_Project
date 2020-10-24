new_chunkostruct( nbufs )
int nbufs;
{
	CHUNK_ORDER_PTR new;
	int i;
	
	if ( !( new = (CHUNK_ORDER_PTR) TALLOC(1, CHUNK_ORDER, "new_chunkostruct") ) 
		 	|| !( new->bufseq = (int *) TALLOC(nbufs, int, "new_chunkostruct") ) )
	{
		pvmlogerror("new_chunkostruct: can't get memory\n");
		pvmbailout(PvmNoMem);	
		return  (CHUNK_ORDER_PTR) NULL;
	}

	/* init the sequence #'s and buffer sequence #'s, nbufs */

	init_chunkostruct( new, nbufs );

	return new;
}
