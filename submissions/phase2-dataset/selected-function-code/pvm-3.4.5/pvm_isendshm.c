pvm_isendshm(sseg, buf, len, dst, tag)
struct segdesc *sseg;
char *buf;
int len;
int dst;
int tag;
{

int freebuf;
int i;
int msgnum;
int offset;
struct msgctrl *control;
struct msginfo *tstmsg;
struct shm_midlist *midentry;
struct shm_midlist *testentry;
int midListId = 0;
char *sbuf;

	if (sseg == (struct segdesc *) NULL)
		return PvmBadParam;		/* invalid shared segment */

	if (buf) /* we are actually trying to send something */
	{
		if ( (midListId = pvm_allocShmmid( &midentry)) < 0)
		return midListId;

		/* record the receive info in the midlist */
		midentry -> ml_status = ISSENDMSG;
		midentry -> ml_dst = dst;
		midentry -> ml_tag = tag;
		midentry -> ml_len = len;
		midentry -> ml_buf = buf;
		midentry -> shm_msgnum = -1;
		midentry -> ml_info = 0;
		midentry -> ml_link = midentry -> ml_rlink = (struct shm_midlist *) NULL;

		/* Put this on the sendq for this shared segment, this maintains
			point-to-point message order */
		if ( !sseg -> sendq )
			sseg -> sendq = midentry;
		else
			LISTPUTBEFORE (sseg -> sendq, midentry, ml_link, ml_rlink);

	}

	testentry = sseg -> sendq; /* get message to send */

	control = sseg->segControl;

	while ( testentry )
	{
		(*sseg->pagelock)(sseg->lock);	/* acquire lock */

		tstmsg = control -> msgs;

		for (i = 0; i < control -> nmsgs; i++)
		{
			if ( tstmsg -> mi_status & ISRECVMSG
					&& ! ( tstmsg -> mi_status & COMPLETE )
					&& ! ( tstmsg -> mi_status & BUFINFLUX ) /*another writer?*/
					&& (tstmsg -> mi_src == MPPANY ||
							tstmsg -> mi_src == testentry->ml_dst)
					&& (tstmsg -> mi_tag == MPPANY ||
							tstmsg -> mi_tag == testentry->ml_tag)
					&& (tstmsg -> mi_len >= testentry -> ml_len) )

			{
				/* record send message information in the shared buffer */
				tstmsg -> mi_status |=  BUFINFLUX; /* write in progress */
				tstmsg -> mi_tag = testentry -> ml_tag;
				tstmsg -> mi_src = pvmmynode;
				tstmsg -> mi_len = testentry -> ml_len;
				sbuf = sseg -> start + tstmsg -> mi_offset;

				/* unlock the control segment while copying */
				(*sseg -> pageunlock)(sseg -> lock);
				BCOPY (sbuf,  testentry -> ml_buf, testentry -> ml_len);
				/* NOTE: control->nmsgs may have increased while unlocked,
					this means that control->nmsgs can never decrease, or
					errors can occur */

				/* relock  to update message that has been copied */

				(*sseg -> pagelock)(sseg -> lock);

				tstmsg -> mi_status &= ~BUFINFLUX;  /* write complete */
				tstmsg -> mi_status |= COMPLETE; 	/* message complete */

				/* record information in our mid structure entry */
				testentry -> ml_status |= COMPLETE;
				break;
			}

			tstmsg ++; /* look to see if another entry can receive this one */
		}

		/* if we were able to complete this message, try to send  next one */
		if ( testentry -> ml_status & COMPLETE )
		{
			if (testentry -> ml_link)
			{
				sseg -> sendq = testentry -> ml_link;
				LISTDELETE (testentry, ml_link, ml_rlink);
				testentry = sseg -> sendq;	/* get the next send message */
			}
			else
		    {
				sseg -> sendq = testentry = ( struct shm_midlist * ) NULL;
			}
		}
	}

	return midListId;	/* Valid message number */
}
