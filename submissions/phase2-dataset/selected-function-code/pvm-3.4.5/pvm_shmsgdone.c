pvm_shmsgdone (midListId)
int midListId;
{
int rval;
struct shm_midlist *midentry;
struct msginfo *tstmsg;
struct segdesc *sseg;

	if (midListId < 1 || midListId >= NSHMMIDS)
		return PvmBadParam;

	midentry =  &(pvm_shmmids[midListId]);

	if (! midentry -> ml_status ) /* No Buffer defined, SEND or RECV */
		return PvmBadParam;

	if ( midentry -> ml_status & ISRECVMSG )  /* This is a receive message */
	{
		if ( midentry -> shm_msgnum < 0) /* invalid shared message num */
			return PvmBadParam;

		sseg = midentry -> shmseg;

		tstmsg = &(sseg->segControl->msgs[ midentry -> shm_msgnum ]);

		(*sseg -> pagelock)(sseg -> lock ); /* lock page and check status */

		if ( tstmsg -> mi_status & COMPLETE )		/* message completed */
		{
			midentry -> ml_status |= COMPLETE;
			midentry -> ml_info[TAGENTRY] = tstmsg -> mi_tag;
			midentry -> ml_info[SRCENTRY] = tstmsg -> mi_src;
			midentry -> ml_info[LENENTRY] = tstmsg -> mi_len;
			tstmsg -> mi_status = NOBUFFER;		/* Free this message buffer*/
		}

		(*sseg -> pageunlock)(sseg -> lock ); /* unlock page  */

	}
	else if (midentry -> ml_status & ISSENDMSG )
	{
		if (! (midentry -> ml_status & COMPLETE))
		{
			/* message isn't complete, try and send again */
			pvm_isendshm( midentry -> shmseg , (char *) NULL,
					0, 0, 0, (int *) NULL);
		}
	}

	/* Check one more time, if the send/recv operation has completed.
		if it has, mark the midlist entry as free and return that the message
		has completed */

	if ((rval = (midentry -> ml_status & COMPLETE)) != 0)
		pvm_freeShmmid( midListId);

	return rval;
}
