int
post_receive(imsg, src, tag, mxsize, headroom, ptype, appid, llmsg)
MSG_INFO_PTR imsg;
int src, tag, mxsize, headroom;
int ptype;
int appid;
MSGFUNC_PTR llmsg;
{
	int cc;

	char *datap;
	char errtxt[128];

	CHUNK_PTR msgchunk;
	
 	if ( !(msgchunk = CHUNK_NEW(mxsize)) )
	{
		pvmlogerror("post_recv: couldn't allocate memory \n");
		return PvmNoMem;
	}	

	datap =  msgchunk->CHUNK_DAT + headroom; 

	imsg -> rchunk = msgchunk;		/* PVM packet */
	imsg -> src = src; 
	imsg -> tag = tag;	/* tag for this buffer */
	imsg -> mxsize = mxsize;	/* max msg size for this buffer */ 
	imsg -> hsize =  headroom; 	/* header size (data offset) */
	
	cc = (*llmsg->imsgrecv)(appid, src, tag, datap, mxsize - headroom, 
			ptype, imsg->info, &(imsg->mid));

	if ( cc < 0)
	{
		
		sprintf(errtxt, "post_recv: bad msg id (%d). Fatal! \n", cc);
		pvmlogerror(errtxt);
		return PvmSysErr;
	}	
}		
