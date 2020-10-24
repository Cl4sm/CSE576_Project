int 
ogm_complete( ogmlist )
struct msgid **ogmlist; 
{

struct msgid *mp, *oldmp, *head;

#if defined(IMA_MPP)
 	if (!(mp = *ogmlist) )
		return TRUE;		/* there is nothing to check */
	
	/* go through the list message ids. checking if the message has
		complete. If the list is empty at the end, then return that
		message has finished */ 

	head = mp;
	mp = mp->ms_link;
	while (mp != head)
	{
		if ((*mp->mfunc->msgdone)(0, &(mp->id), mp->info))
		{
			mp = mp->ms_link;
			oldmp = mp->ms_rlink;
			LISTDELETE(oldmp,ms_link, ms_rlink)
			msgid_free(oldmp);
		}
		else
			mp = mp->ms_link;
		
	}

	/* mp now points to the head of the list */
	if ((*mp->mfunc->msgdone)(0, &(mp->id), mp->info))
	{
		if (mp->ms_link == mp)	/* the only one left */
		{
			msgid_free(mp);
			*ogmlist = (struct msgid *) NULL;
			return TRUE;
		}
		else
		{
			mp = mp->ms_link;
			oldmp = mp->ms_rlink;
			LISTDELETE(oldmp, ms_link, ms_rlink);
			msgid_free(oldmp);
			*ogmlist = mp;	/* new head of the message id list */
		}
	}	
	return FALSE;
#else
	return TRUE;	/* for workstations, the write is sychronous */
#endif
}
