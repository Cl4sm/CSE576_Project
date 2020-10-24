new_vdirectstruct( nstructs, nsbufs, nrbufs )
int nsbufs;
int nrbufs;
{
	MPP_DIRECTI_PTR vnew, current;
	CHUNK_ORDER_PTR ostructs;
	int i,j;
	
	if (  !( vnew = (MPP_DIRECTI_PTR) TALLOC(nstructs, MPP_DIRECTI, "new_vdirectstruct") ) 
		 	|| !( ostructs = new_vchunkostruct( nstructs, nrbufs ) ) )
	{
		pvmlogerror("new_vdirectstruct: can't get memory\n");
		pvmbailout(PvmNoMem);	
		return  (MPP_DIRECTI_PTR) NULL;
	}

	/* Initialize the variables. Set unknown values to -1 */

	for (j = 0; j < nstructs; j++)
	{
		current = vnew + j;

		init_directstruct( current, nsbufs);

		/* point to one of the ordering structs that we just malloc'ed */
		current -> ordering = ostructs + j;
	}


	return vnew;
}
