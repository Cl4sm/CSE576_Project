int 
pvm_mpppsend(cp, len, tid, tag)
char *cp;
int len;
int tid;
int tag;
{

	static char nullusrmsg;
	static int first = 1;
	static int psbuf;
	static struct frag *dfrag;
	static struct pmsg *psmsg;
	static struct timeval ztv = { 0, 0 };
	int savebuf;
	int cc;

	/* First call to psend will create a regular pvm buffer for packing.
	 *	o	A new message is created as a by-product of pvm_mkbuf.
	 * 	o	A single (inplace) data buffer is created and put in the frag chain 
	 *	o	Later calls will simply twiddle the message buffer and the frag
	 */
	if (first)
	{
		psbuf = pvm_mkbuf(PvmDataInPlace); /* Make an inplace data buffer */
	    savebuf = pvm_setsbuf(psbuf);
		pvm_pkint(&cc, 1, 1);			/* create a single frag for msg */ 
		pvm_setsbuf(savebuf);
		psmsg = midtobuf(psbuf);		/* save a pointer to the msg struct */
		dfrag = psmsg->m_frag->fr_link;
		first = 0;
	}

	psmsg->m_ctx = pvmmyctx;			/* set the context */

	if (len)
	{
		dfrag->fr_buf = dfrag->fr_dat = cp;
		dfrag->fr_max = dfrag->fr_len = len;
	}
	else
	{
		dfrag->fr_buf = dfrag->fr_dat = &nullusrmsg;
		dfrag->fr_max = dfrag->fr_len = len;
	}

	cc = mroute(psbuf, tid, tag, &ztv); /* route the message */ 
	
	return cc;
}
