int
pvm_irecvshm(sseg, buf, len, src, tag, info)
struct segdesc *sseg;
char *buf;
int len;
int src;
int tag;
int *info;
{

int freebuf;
int msgnum;
int offset;
struct msgctrl *control;
struct msginfo *thismsginfo;
struct shm_midlist *midentry;
int midListId;

	if (sseg == (struct segdesc *) NULL)
		return PvmBadParam;		/* invalid shared segment */

	if ( (offset = (buf -  sseg -> start)) < 0
			|| (sseg -> start + sseg -> len) < (buf + len) )
		return PvmBadParam;		/* data buffer not completely in shared seg */

	if ( (midListId = pvm_allocShmmid( &midentry)) < 0)
		return midListId;

	/* record the receive info in the midlist */
	midentry -> ml_status = ISRECVMSG;
	midentry -> ml_src = src;
	midentry -> ml_tag = tag;
	midentry -> ml_len = len;
	midentry -> ml_buf = buf;
	midentry -> shm_msgnum = -1;
	midentry -> ml_info = info;
	midentry -> ml_link = midentry -> ml_rlink = (struct shm_midlist *) NULL;


	(*sseg->pagelock)(sseg->lock);	/* acquire lock */

	control = sseg->segControl;

	/* determine which msginfo buffer we can use for this message */
	freebuf = 0;
	for (msgnum = 0; msgnum < control->nmsgs; msgnum++ )
	{
		freebuf = control->msgs[msgnum].mi_status == NOBUFFER;
		if (freebuf)
			break;
	}

	/* If no free msginfo buffers, Space for one more ? */
	if (msgnum == control->nmsgs)
	{
		if (control -> nmsgs >= control -> maxmsgs)  /* too many posted recv's*/
		{
			midentry -> ml_status = NOBUFFER; 	/* free the midentry */
			(*sseg->pageunlock)(sseg->lock);	/* release lock */
			return PvmSysErr;			/* out of mids */
		}
		else
			control-> nmsgs ++;
	}

	/* Set the msginfo buffer parameters */
	thismsginfo = &(control->msgs[msgnum]);
	thismsginfo -> mi_src = src;
	thismsginfo -> mi_tag = tag;
	thismsginfo -> mi_len = len;
	thismsginfo -> mi_offset = offset;
	thismsginfo -> mi_midlistId = midListId;
	thismsginfo -> mi_status = ISRECVMSG;

	(*sseg->pageunlock)(sseg->lock); /* release lock */

	return midListId;	/* Valid message number */
}
