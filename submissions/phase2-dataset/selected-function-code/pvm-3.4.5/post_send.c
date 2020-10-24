post_send(buffer, len, distruct, llmsg)
char *buffer;
int len;
MPP_DIRECTI_PTR distruct;
MSGFUNC_PTR llmsg;
{
	int appid;
	int cc;
	int dest;
	int ptype;
	int rtid;
	int tag; 

	msgmid_t mid;

	if (distruct == (MPP_DIRECTI_PTR) NULL)
		return (-1);

	rtid = distruct->rtid;

	tag = distruct -> sseq % distruct -> tagbase;

	dest = (distruct -> rtid) & TIDNODE;

	ptype = (distruct -> rpid); 
	
	appid = (distruct -> appid);

	/* Increment the send sequence number if the send is successful */
	if ( (cc =  (*llmsg->imsgsend)(appid, tag, buffer, len, dest, ptype, &mid)) >= 0 )
		distruct -> sseq = INCRSEQ( distruct->sseq, MAXSEQ); 

	return mid;
}	
