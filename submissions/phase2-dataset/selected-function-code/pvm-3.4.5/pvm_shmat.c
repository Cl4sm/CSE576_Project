pvm_shmat ( sseg )
struct segdesc *sseg;
{
char *buffer;
	if (sseg->bound)
		return(sseg->start);

	if (!sseg->lock)
	{
		pvmlogerror( "pvm_shmat(): need to get lock before attach\n") ;
		return (char *) NULL;
	}

	if ( (buffer = shmat(sseg->segid, (char *) NULL, 0) ) == (char *) NULL)
	{
		pvmlogerror( "pvm_shmat() : could not attach to the share segment\n");
		return (char *) NULL;
	}


	sseg -> start = buffer;
	sseg -> segControl = (struct msgctrl *) buffer;


	/* Initialize and/or Update msgctrl section of this shared segment */

	(*sseg->pagelock) (sseg->lock);

	if (sseg -> iAmOwner)
		pvm_msgctrlInit(sseg -> segControl, sseg->len,
				MAXFRAGSIZE, MAXFRAGSIZE);

	sseg->segControl->nattach ++; /* our idea of # tasks attached */

	(*sseg->pageunlock) (sseg->lock);

	sseg->bound = 1;

	return buffer;
}
