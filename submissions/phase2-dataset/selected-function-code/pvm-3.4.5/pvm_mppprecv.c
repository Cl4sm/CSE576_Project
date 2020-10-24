int
pvm_mppprecv(tid, tag, cp ,len, dt, rtid, rtag, rlen)
	int tid;
	int tag;
	void *cp;
	int len;
	int dt;
	int *rtid;
	int *rtag;
	int *rlen;
{
	char errtxt[64];

	int rbf;
	int cc = 0;
	int l;
	int x;
	long ad;

	static int first = 1;
	static struct msgid * thisprecv;

	/* allocate a new message id for this precv */

	if (first)
	{
		if ( !(thisprecv = msgid_new()))
		{
			pvmlogerror("mppprecv(): can't get memory \n");
			return PvmNoMem;
		}
		first = 0;
	}
 	

	thisprecv -> otid =  tid;
	thisprecv -> tag = tag;
	thisprecv -> ctxt = pvm_getcontext();
	thisprecv -> ubuf = cp;
	thisprecv -> len = len; 
	thisprecv -> complete = 0;

	pvm_inprecv = thisprecv;
	
	if (!cc) {
	
		cc = pvm_recv(tid, tag);

		if (cc >= 0) {

			if (!(thisprecv->complete)) /* receive went into pvm buffers */
			{		
				pvm_bufinfo(cc, &l, rtag, rtid);
				if 	(rlen)
					*rlen = l;
				if (l < len)
					len = l;
				if (pvmdebmask & PDMMESSAGE) {
					sprintf(errtxt, "precv() unpacking message len = %d\n", l);
					pvmlogerror(errtxt);
				}
				pvm_upkbyte((char *)cp, len, 1);
			}
			else /* receive went straight into user memory */
			{
				if(rlen)
					*rlen = thisprecv -> len;
				if (pvmdebmask & PDMMESSAGE) {
					sprintf(errtxt, "precv() short ckted into user buf len = %d\n", l);
					pvmlogerror(errtxt);
				}
			}
			cc = 0;
		}
	}

	pvm_inprecv = (struct msgid *) NULL;

	if (cc < 0)
		lpvmerr("pvm_mppprecv", cc);
	return cc;
}
