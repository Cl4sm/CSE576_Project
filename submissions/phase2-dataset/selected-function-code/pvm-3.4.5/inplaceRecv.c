inplaceRecv(src, tag, buf, len, rlen, llmsg)
int src;
int tag;
char *buf;
int len;
int *rlen;
MSGFUNC_PTR llmsg;
{
 	MSG_INFO imsg;		

	int appid = 0;		/* ignored by tasks */
	int cc;
	int ptype = 0;		/* ouch! XXX. Fix this ! */

	char logtxt[64];

 	cc = (*llmsg->imsgrecv)(0, src, tag, buf,len, ptype, imsg.info, &(imsg.mid));

	if (cc < 0)
	{
		sprintf(logtxt," inplaceRecv(): bad mid (%d)\n", cc);
		pvmlogerror(logtxt);
		return PvmSysErr;
	}

	while ( (cc = (*llmsg->msgdone)(appid, &(imsg.mid), imsg.info)) == 0 )
		/* wait for recv to finish */ ; 

	if (cc < 0)
 	{
		sprintf(logtxt,"inplaceRecv(): error on msgdone (%d)\n", cc);
		pvmlogerror(logtxt);
		return PvmSysErr;
	}

	*rlen = (*llmsg->msglen)(imsg.info);

	return 0;

}
