int
mpp_input()
{
	static int cbuf = 0;
	struct pkt *pp = 0;

	int buf; 				/* buffer we are working on */
	int mxbufs = NRBUFS;	/* maximum number of buffers we will proc on read */
	int npkts = 0;
	int tsrc, src;
	int tag;
	int pid = 0;
	int mid;

	MPP_DIRECTI_PTR tcon;

	static CHUNK_PTR readyList = (CHUNK_PTR) NULL;

	struct pkt *hdr; 

	if ( mprecvbufs == (MSG_INFO_PTR) NULL )  
	{
		/* we haven't alloc'ed any buffers. No tasks loaded */
		return npkts;
	}

	for (buf = 0; buf < mxbufs; buf++) /* limit #of packets to read */
	{
		if (pp = (struct pkt *) pvm_chunkReady(mprecvbufs, mxbufs,
				pvm_hostmsgfunc(), mpdirect, partsize, &cbuf,
				&readyList) )
		{
#if defined(IMA_PGONPUMA)
			src = extract_lnid(src);
#endif
			npkts ++;

			/* Step 2) extract header information from the packet */

			pp->pk_dst = pvmget32(pp->pk_dat);
			pp->pk_src = pvmget32(pp->pk_dat + 4);
			pp->pk_flag = pvmget8(pp->pk_dat + 12);
			pp->pk_len -= TDFRAGHDR;
			pp->pk_dat += TDFRAGHDR;
			if (pp->pk_flag & FFSOM) 
			{
				if (pp->pk_len < MSGHDRLEN) 
				{
					sprintf(etext,
							"mpp_input() SOM pkt src t%x dst t%x short\n",
							pp->pk_src, pp->pk_dst);
					pvmlogerror(etext);
					pk_free(pp);
					return;
				}
				pp->pk_enc = pvmget32(pp->pk_dat);
				pp->pk_tag = pvmget32(pp->pk_dat + 4);
				pp->pk_ctx = pvmget32(pp->pk_dat + 8);
				pp->pk_wid = pvmget32(pp->pk_dat + 16);
				pp->pk_crc = pvmget32(pp->pk_dat + 20);
				pp->pk_len -= MSGHDRLEN;
				pp->pk_dat += MSGHDRLEN;
			}
		
			/* Step 3) Deliver packet to the PVM processing input stream */

			loclinpkt((struct task *)0, pp);

		}
		else
			break;	/* no packets to read */

	}

	return npkts;
}
