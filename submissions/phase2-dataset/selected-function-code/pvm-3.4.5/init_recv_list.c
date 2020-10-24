init_recv_list( nbufs, tagbase, msize, hsize, appid, llmsg )
int nbufs, tagbase, msize, hsize;
int appid;
MSGFUNC_PTR llmsg;
{
	MSG_INFO_PTR new, tmp;
	CHUNK_PTR msgchunk;
	char * datap;
	int i;	
	int cc;
	int mrsize; /* message receive size */

	if (  !( new = (MSG_INFO_PTR) TALLOC(nbufs, MSG_INFO, "init_recv_list") ) ) 
	{
		pvmlogerror("init_recv_list: couldn't allocate memory \n");
		return (MSG_INFO_PTR) NULL;
	}

	tmp = new;
	
	for (i = 0; i < nbufs; i ++)
	{
		cc = post_receive(tmp, -1, tagbase + i, msize, hsize, MPPANY, appid,
			llmsg);
		tmp ++;
		if (cc < 0)
		{	
			sprintf(mpperrtext,"init_recv_list(): post_receive failed! src %d tag %d msize %d hsize %d pid %d appid %d : nbufs %d buffer %d \n", 
			-1, tagbase + i, msize, hsize, MPPANY, appid, nbufs, i);
		
			pvmlogerror(mpperrtext);
		}
	}

	return new;
}
