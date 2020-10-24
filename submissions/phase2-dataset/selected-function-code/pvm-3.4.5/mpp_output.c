int
mpp_output(tp, pp)
	struct task *tp;
	struct pkt *pp;
{
	char *cp;
	int len;
	long node;				/* node number */
	long ptype;				/* process type */
	struct nodeset *sp;

	int cc;
	int i;
	int idx1;
	int tag;

	struct pkt * delpkt;
	struct pkt * npkt;
	struct pkt * pp2;
	struct pkt * tstpkt;
	struct task *tp2;

	MPP_DIRECTI_PTR tdirect;

	MSGFUNC_PTR	mfunc;

	mfunc = pvm_hostmsgfunc();

	/* check if we have any packets to queue up and send */
	if (tp && pp )
	{
		node = tp->t_tid & tidnmask;

		ptype = (tp->t_tid & tidtmask) >> (ffs(tidtmask) - 1);

#if defined(IMA_PGON)
		/* XXX ptype is not used for PUMA host-node communication */
		for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link)
		{
			if ((sp->n_ptype & ptypemask) == ptype) 
			{
				ptype = sp->n_ptype;
				break;
			}
		}
		if (sp == busynodes) 
		{
			sprintf(etext, "mpp_output() pkt to t%x scrapped (no ptype %ld)\n",
				tp->t_tid, ptype);
			pvmlogerror(etext);
			pk_free(pp);
			goto done;
		}

#endif
		cp = pp->pk_dat;

		len = pp->pk_len;

		if (pp->pk_flag & FFSOM) 
		{
			cp -= MSGHDRLEN;

			len += MSGHDRLEN;

			if (cp < pp->pk_buf) {
				pvmlogerror("mpp_output() no headroom for message header\n");
				pk_free(pp);
				goto done;
			}
			/* If we are not an MCA'ed pkt OR FFMCAWH (MCA write hdr)
											is set, then write the header */
			if ( !(pp->pk_flag & FFMCA)  ||  pp->pk_flag & FFMCAWH)
			{
				pvmput32(cp, pp->pk_enc);
				pvmput32(cp + 4, pp->pk_tag);
				pvmput32(cp + 8, pp->pk_ctx);
				pvmput32(cp + 16, pp->pk_wid);
				pvmput32(cp + 20, pp->pk_crc);
				if (pvmdebmask & PDMMCA)
				{
					sprintf(etext, "mpp_output wrt TDMSGHDR for %x\n", pp->pk_buf);
					pvmlogerror(etext);
				} 
			}

		}
		if (pvmdebmask & PDMPACKET) {
			sprintf(etext,
				"mpp_output() src t%x dst t%x ff %x len %d ptype %d\n",
				pp->pk_src, pp->pk_dst, pp->pk_flag & (FFSOM|FFEOM),
				len, ptype);
			pvmlogerror(etext);
		}
		cp -= TDFRAGHDR;
		if (cp < pp->pk_buf) {
			pvmlogerror("mpp_output() no headroom for packet header\n");
			pk_free(pp);
			goto done;
		}
		/* If we are not an MCA'ed pkt OR FFMCAWH (MCA write hdr)
									is set, then write the header */
		if ( !(pp->pk_flag & FFMCA)  ||  pp->pk_flag & FFMCAWH)
		{
			pvmput32(cp, pp->pk_dst);
			pvmput32(cp + 4, pp->pk_src);
			pvmput32(cp + 8, len);
			pvmput32(cp + 12, 0);
			pvmput8(cp + 12, pp->pk_flag & (FFSOM|FFEOM|FFMCA));
			if (pvmdebmask & PDMMCA)
			{
				sprintf(etext, "mpp_output wrt TDFRAGHDR for %x\n", pp->pk_buf);
				pvmlogerror(etext);
			} 
		}
		len += TDFRAGHDR;

		pp->pk_dat = cp; 	/* store where the data starts */ 
		pp->pk_len = len;	/* store the length of the buffer */

		/* okay we've done everything to the packet, now put it on the output
			queue */

		LISTPUTBEFORE(mppopq, pp, pk_link, pk_rlink);
	}

	tstpkt = mppopq -> pk_link;
	while (tstpkt != mppopq)
	{
		/* find free index */
		if ( (idx1 = pvm_mpp_find_midx(mppsendmids, (CHUNK_PTR *) mppoutpkts, 
					&lastidx, NMPPSBUFMIDS, mfunc) ) >= 0 ) 
		{
			node = tstpkt->pk_dst & tidnmask;

			tdirect = mpdirect + node;

			ptype = (tstpkt->pk_dst & tidtmask) >> (ffs(tidtmask) - 1);

			tag = PMTDBASE + tdirect->sseq;

			if (++(tdirect->sseq) >= tdirect->nbufs)
				tdirect->sseq = 0;

			/* send the actual packet of information */
			/* Hmm, should check here for long packets  ... ?? */
			/* XXX appid needs to be checked here for PUMA */ 
#if defined(IMA_PGONPUMA)
			ptype = extract_pid(appid, node);
			node = extract_nid(appid, node);
#endif
			
			len = tstpkt->pk_len;

			if (len < 16)
			{
				sprintf(etext,"mpp_output() bogus len %d? (dst %x, ff %d)\n",
					len, tstpkt->pk_dst, tstpkt->pk_flag);
		
				pvmlogperror(etext);
			}
			(*mfunc->imsgsend)(appid, tag, tstpkt->pk_dat, len, 
					node, ptype, &cc); 
			pvm_assign_mid(mppsendmids, cc, idx1);
			if (cc < 0 ) 
			{
				sprintf(etext,"mpp_output() can't send to t%x", tstpkt->pk_dst);
				pvmlogperror(etext);

				/* clear the output queue of packets for this tid in the output
				   queue */	
				npkt = tstpkt->pk_link;
				while (npkt != tstpkt)
				{
					if ( npkt->pk_dst == tstpkt->pk_dst)
					{
						delpkt = npkt;
						npkt = npkt->pk_link;
						LISTDELETE(delpkt,pk_link, pk_rlink);
						pk_free(delpkt);
					}
					else
						npkt = npkt->pk_link;
				} /* mppopq queue is cleared */

				delpkt = tstpkt;

				tstpkt = tstpkt->pk_link;

				LISTDELETE(delpkt, pk_link, pk_rlink);

				tp2 = task_find(delpkt->pk_dst);	

				pk_free(delpkt);

				mpp_free(tp);

				tp2 = tp2->t_rlink;

				task_cleanup(tp2->t_link);

				task_free(tp2->t_link);

			} 
			else  /* isend was ok, remember the pkt, go onto the next one */
			{
				pvm_assign_chunk((CHUNK_PTR *) mppoutpkts, (CHUNK_PTR) tstpkt,
								 idx1);

				if (pvmdebmask & (PDMPACKET | PDMNODE)) 
				{
					sprintf(etext,
						"mpp_output() from mppopq src t%x dst t%x len %d ptype %d flags %s\n",
						tstpkt->pk_src, tstpkt->pk_dst, len, ptype,
						pkt_flags(tstpkt->pk_flag));
					pvmlogerror(etext);
				}

				/* remove the packet from the mppopq */ 
				delpkt = tstpkt;
				tstpkt = tstpkt->pk_link;

				LISTDELETE(delpkt,pk_link,pk_rlink);
			}
		}
		else
		{
			if (pvmdebmask & PDMPACKET) {
				sprintf(etext,
					"mpp_output() no free send message ids.\n");
				pvmlogerror(etext);
			}
			pvmlogerror("(*)\n");
			return 0;
		}
	}
done:
	if (tp && tp->t_flag & TF_CLOSE) {
	    mpp_free(tp);
		tp = tp->t_rlink;
/* XXX tm_exit() also calls task_cleanup(); should only be done once */
		task_cleanup(tp->t_link);
		task_free(tp->t_link);
	}
	return 0;
}
