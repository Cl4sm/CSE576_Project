new_directstruct( nrbufs, nsbufs )
int nrbufs;  /* number of receive buffers */
int nsbufs;	 /* number of send buffers */
{
	MPP_DIRECTI_PTR new;
	int i;
	
	if ( !( new = (MPP_DIRECTI_PTR) TALLOC(1, MPP_DIRECTI, "new_directstruct") )
			|| ! (new -> ordering = new_chunkostruct( nrbufs )))
	{
		pvmlogerror("new_directstruct: can't get memory\n");
		pvmbailout(PvmNoMem);	
		return  (MPP_DIRECTI_PTR) NULL;
	}

	init_directstruct( new,  nsbufs);
	
	return new;

}
