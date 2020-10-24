read_chunk( imsg, src, tag, len , repost, appid, llmsg)
MSG_INFO_PTR imsg;
int *src;
int *tag;
int *len;
int repost;
int appid;
MSGFUNC_PTR llmsg;
{
	CHUNK_PTR rchunk= (CHUNK_PTR) NULL;

	if (imsg && imsg->rchunk && imsg->mid >= 0 
			&& (*llmsg->msgdone)(appid, &(imsg->mid), imsg->info))

	{
		*src = (*llmsg->msgsrc)(imsg->info);
		*len = (*llmsg->msglen)(imsg->info);
		*tag = (*llmsg->msgtag)(imsg->info);
		rchunk = imsg -> rchunk;

		if (pvmdebmask & PDMPACKET)
		{
			sprintf(mpperrtext,"read-chunk(): node %d, len %d, tag %d \n", 
					*src, *len, *tag);
			pvmlogerror(mpperrtext);
		}

		if (repost)
			post_receive(imsg, imsg->src, imsg->tag, imsg->mxsize, imsg->hsize, 					MPPANY, appid, llmsg);
	}
		
	return rchunk;		
}
