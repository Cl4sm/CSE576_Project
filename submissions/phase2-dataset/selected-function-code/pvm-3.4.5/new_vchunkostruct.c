new_vchunkostruct( nstructs, nbufs )
int nstructs;
int nbufs;
{
	CHUNK_ORDER_PTR vnew, current;
	int *bufs;
	int i,j;
	
	if (  !( vnew = (CHUNK_ORDER_PTR) TALLOC(nstructs, CHUNK_ORDER, "new_vchunkostruct") ) 
		 	|| !( bufs = (int *) TALLOC(nbufs * nstructs, int, "new_vchunkostruct") ) )
	{
		pvmlogerror("new_vchunkostruct: can't get memory\n");
		pvmbailout(PvmNoMem);	
		return  (CHUNK_ORDER_PTR) NULL;
	}

	/* init the sequence #'s and buffer sequence #'s, nbufs */

	for (j = 0; j < nstructs; j++)
	{
		current = vnew + j;
		current -> bufseq = bufs + ( nbufs * j ); 
		init_chunkostruct( current, nbufs );
	}

	return vnew;
}
