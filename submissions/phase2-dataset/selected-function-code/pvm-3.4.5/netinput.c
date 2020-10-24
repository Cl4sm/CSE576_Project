netinput()
{
	struct sockaddr_in osad;		/* sender's ip addr */
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
		|| defined(IMA_AIX56K64) || defined(IMA_LINUXALPHA)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif						/* sockaddr length */
	struct timeval tnow;
	struct pkt *pp, *pp2;
	struct hostd *hp;
	char *cp;
	int sqn;
	int aqn;
	int ff;
	int dst;
	int src;
	int hh;
	int already;
	struct timeval tdiff;			/* packet rtt */
	int rttusec;

	/*
	* alloc new pkt buffer and read packet
	*/

	pp = pk_new(pvmudpmtu);
	if (TDFRAGHDR > DDFRAGHDR)
		pp->pk_dat += TDFRAGHDR - DDFRAGHDR;

	oslen = sizeof(osad);
	if ((pp->pk_len = recvfrom(netsock, pp->pk_dat,
			pp->pk_max - (pp->pk_dat - pp->pk_buf),
			0, (struct sockaddr*)&osad, &oslen)) == -1) {
		if (errno != EINTR)
			pvmlogperror("netinput() recvfrom(netsock)");
		goto scrap;
	}

#if 0
	/* drop random packets */
	if (!(random() & 3)) {
		pvmlogerror("netinput() oops, dropped one\n");
		goto scrap;
	}
#endif

#ifdef	STATISTICS
	stats.rfok++;
#endif

	cp = pp->pk_dat;
	pp->pk_len -= DDFRAGHDR;
	pp->pk_dat += DDFRAGHDR;
	dst = pp->pk_dst = pvmget32(cp);
	src = pp->pk_src = pvmget32(cp + 4);
	sqn = pp->pk_seq = pvmget16(cp + 8);
	aqn = pvmget16(cp + 10);
	ff = pp->pk_flag = pvmget8(cp + 12);
	if (ff & FFSOM) {
		if (pp->pk_len < MSGHDRLEN) {
			pvmlogprintf("netinput() SOM pkt src t%x dst t%x too short\n",
					src, dst);
			goto scrap;
		}
		cp += DDFRAGHDR;
		pp->pk_enc = pvmget32(cp);
		pp->pk_tag = pvmget32(cp + 4);
		pp->pk_ctx = pvmget32(cp + 8);
		pp->pk_wid = pvmget32(cp + 16);
		pp->pk_crc = pvmget32(cp + 20);
		pp->pk_len -= MSGHDRLEN;
		pp->pk_dat += MSGHDRLEN;
	}

	/*
	* make sure it's from where it claims
	*/

	hh = (src & tidhmask) >> (ffs(tidhmask) - 1);
	if (hh < 0 || hh > hosts->ht_last || !(hp = hosts->ht_hosts[hh])
/* #ifndef IMA_LINUX */
/* appears to be O.K. now in RedHat 5.0... JAK 1/28/98 */
	/*
	* XXX removing these lines is a hack and reduces security between
	* XXX pvmds somewhat, but it's the easiest fix for Linux right now.
	*/
	|| (osad.sin_addr.s_addr != hp->hd_sad.sin_addr.s_addr)
	|| (osad.sin_port != hp->hd_sad.sin_port)
/* #endif */
	) {
		pvmlogprintf("netinput() bogus pkt from %s\n",
				inadport_decimal(&osad));
		goto scrap;
	}

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf(
		"netinput() pkt from %s src t%x dst t%x f %s len %d seq %d ack %d\n",
				hp->hd_name, src, dst, pkt_flags(ff), pp->pk_len, sqn, aqn);
	}

	if ((ff & (FFFIN|FFACK)) == (FFFIN|FFACK)) {
		if (hh == hosts->ht_master) {
	/*
	* FIN|ACK from master means we should bailout
	*/
			if (runstate == PVMDPRIME) {
				if (pvmdebmask & PDMSTARTUP)
					pvmlogerror("work() PVMDPRIME halting\n");
				exit(0);
			}
			pvmlogprintf("netinput() FIN|ACK from master (%s)\n",
					hp->hd_name);
			runstate = PVMDHALTING;

		} else {
	/*
	* FIN|ACK from slave means it croaked
	*/
			pvmlogprintf("netinput() FIN|ACK from %s\n",
					hp->hd_name);
			hd_dump(hp);
			hostfailentry(hp);
			clear_opq_of((int)(TIDPVMD | hp->hd_hostpart));
			if (hp->hd_hostpart) {
				ht_delete(hosts, hp);
				if (newhosts)
					ht_delete(newhosts, hp);
			}
		}
		goto scrap;
	}

	/*
	* done with outstanding packet covered by this ack
	*/

	if (ff & FFACK) {
		for (pp2 = hp->hd_opq->pk_link; pp2 != hp->hd_opq; pp2 = pp2->pk_link)
			if (pp2->pk_seq == aqn) {
				if (pp2->pk_flag & FFDAT) {
					if (pp2->pk_nrt == 1) {
						pvmgetclock(&tnow);

						TVXSUBY(&tdiff, &tnow, &pp2->pk_at);
						rttusec = tdiff.tv_sec * 1000000 + tdiff.tv_usec;
						if (rttusec < 1)
							rttusec = 1000;	/* XXX const */
						else
							if (rttusec > DDMAXRTT*1000000)
								rttusec = DDMAXRTT*1000000;
						rttusec += 3 * (hp->hd_rtt.tv_sec * 1000000 + hp->hd_rtt.tv_usec);
						rttusec /= 4;
						hp->hd_rtt.tv_sec = rttusec / 1000000;
						hp->hd_rtt.tv_usec = rttusec % 1000000;
					}
				}
				if (pp2->pk_flag & FFFIN) {
					finack_to_host(hp);
				}
				hp->hd_nop--;
				LISTDELETE(pp2, pk_link, pk_rlink);
				pk_free(pp2);
				break;
			}
	}

	/*
	* move another pkt to output q
	*/

/*
	if ((hp->hd_opq->pk_link == hp->hd_opq)
*/
	if (hp->hd_nop < nopax
	&& (hp->hd_txq->pk_link != hp->hd_txq)) {
		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("netinput() pkt to opq\n");
		}
		pp2 = hp->hd_txq->pk_link;
		LISTDELETE(pp2, pk_link, pk_rlink);
		TVCLEAR(&pp2->pk_rtv);
		TVXADDY(&pp2->pk_rta, &hp->hd_rtt, &hp->hd_rtt);
		TVCLEAR(&pp2->pk_rto);
		TVCLEAR(&pp2->pk_at);
		pp2->pk_nrt = 0;
		pp2->pk_hostd = hp;
		pp2->pk_seq = hp->hd_txseq;
		hp->hd_txseq = NEXTSEQNUM(hp->hd_txseq);
		LISTPUTBEFORE(hp->hd_opq, pp2, pk_link, pk_rlink);
		hp->hd_nop++;
		LISTPUTBEFORE(opq, pp2, pk_tlink, pk_trlink);
	}

	if (!(ff & (FFDAT|FFFIN)))
		goto scrap;

	/*
	* send an ack for the pkt
	*/

	pp2 = pk_new(DDFRAGHDR);	/* XXX could reref a dummy databuf here */
	pp2->pk_dat += DDFRAGHDR;
	pp2->pk_dst = hp->hd_hostpart | TIDPVMD;
	pp2->pk_src = pvmmytid;
	pp2->pk_flag = FFACK;
	TVCLEAR(&pp2->pk_rtv);
	TVCLEAR(&pp2->pk_rta);
	TVCLEAR(&pp2->pk_rto);
	TVCLEAR(&pp2->pk_at);
	pp2->pk_nrt = 0;
	pp2->pk_hostd = hp;
	pp2->pk_seq = 0;
	pp2->pk_ack = sqn;
	/*
	 * Bogus!  Acks can't be put at end of opq, as could be
	 * stuck behind packet retries with non-zero pk_rtv's...
	 * Better place Acks, in order, after other zero-rtv-ed (new)
	 * packets, but *before* any retry packets...
	 */
	/* LISTPUTAFTER(opq, pp2, pk_tlink, pk_trlink); */
	{
		struct pkt *pp3;
		for (pp3 = opq->pk_tlink; pp3 != opq; pp3 = pp3->pk_tlink)
			if (TVXLTY(&pp2->pk_rtv, &pp3->pk_rtv))
				break;
		LISTPUTBEFORE(pp3, pp2, pk_tlink, pk_trlink);
	}

	if (!(ff & FFDAT))
		goto scrap;

	/*
	* if we don't have it already, put it in reordering q
	*/

	pp2 = 0;
	if (SEQLESSTHAN(sqn, hp->hd_rxseq))
		already = 1;
	else {
		already = 0;
		for (pp2 = hp->hd_rxq->pk_link; pp2 != hp->hd_rxq; pp2 = pp2->pk_link)
			if (!SEQLESSTHAN(pp2->pk_seq,sqn)) {
				if (pp2->pk_seq == sqn)
					already = 1;
				break;
			}
	}
	if (already) {
		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("netinput() pkt resent from %s seq %d\n",
					hp->hd_name, sqn);
		}
		goto scrap;
	}

	LISTPUTBEFORE(pp2, pp, pk_link, pk_rlink);

	/*
	* accept pkts from reordering q
	*/

	while (pp = hp->hd_rxq->pk_link,
			pp != hp->hd_rxq && pp->pk_seq == hp->hd_rxseq) {
		hp->hd_rxseq = NEXTSEQNUM(hp->hd_rxseq);
		LISTDELETE(pp, pk_link, pk_rlink);
		netinpkt(hp, pp);
	}
	return 0;

scrap:
	if (pp)
		pk_free(pp);
	return 0;
}
