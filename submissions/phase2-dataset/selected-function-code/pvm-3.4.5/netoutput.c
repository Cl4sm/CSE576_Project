netoutput()
{
	struct timeval tnow, tx;
	struct pkt *pp, *pp2;
	struct hostd *hp;
	char *cp;
	int len;
	int cc;
	char dummy[DDFRAGHDR];

/*
	len = 0;
	for (pp = opq->pk_tlink; pp != opq; pp = pp->pk_tlink)
		len++;
	pvmlogprintf("netoutput() %d in opq\n", len);
*/
	if (opq->pk_tlink == opq)
		return 0;

	/*
	* send any pkts whose time has come
	*/

	pvmgetclock(&tnow);

	while ((pp = opq->pk_tlink) != opq && TVXLTY(&pp->pk_rtv, &tnow)) {

	/*
	* fail if we've tried too hard
	*/
		hp = pp->pk_hostd;
		if (pp->pk_nrt >= DDMINRETRIES
		&& pp->pk_rto.tv_sec >= DDMINTIMEOUT) {		/* host is toast */
			pvmlogprintf(
					"netoutput() timed out sending to %s after %d, %d.%06d\n",
					hp->hd_name, pp->pk_nrt,
					pp->pk_rto.tv_sec, pp->pk_rto.tv_usec);
			hd_dump(hp);
			hostfailentry(hp);
			clear_opq_of((int)(TIDPVMD | hp->hd_hostpart));
			ht_delete(hosts, hp);
			if (newhosts)
				ht_delete(newhosts, hp);
			continue;
		}

		cp = pp->pk_dat;
		len = pp->pk_len;
		if (pp->pk_flag & FFSOM) {
			cp -= MSGHDRLEN;
			len += MSGHDRLEN;
			if (cp < pp->pk_buf) {
				pvmlogerror("netoutput() no headroom for message header\n");
				return 0;
			}
			pvmput32(cp, pp->pk_enc);
			pvmput32(cp + 4, pp->pk_tag);
			pvmput32(cp + 8, pp->pk_ctx);
			pvmput32(cp + 16, pp->pk_wid);
			pvmput32(cp + 20, pp->pk_crc);
		}
		cp -= DDFRAGHDR;
		len += DDFRAGHDR;

	/*
	* save under packet header, because databuf may be shared.
	* we don't worry about message header, because it's only at the head.
	*/
		BCOPY(cp, dummy, sizeof(dummy));
		if (cp < pp->pk_buf) {
			pvmlogerror("netoutput() no headroom for packet header\n");
			return 0;
		}

		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf(
			"netoutput() pkt to %s src t%x dst t%x f %s len %d seq %d ack %d retry %d\n",
					hp->hd_name, pp->pk_src, pp->pk_dst, pkt_flags(pp->pk_flag),
					pp->pk_len, pp->pk_seq, pp->pk_ack, pp->pk_nrt);
		}
		pvmput32(cp, pp->pk_dst);
		pvmput32(cp + 4, pp->pk_src);
		pvmput16(cp + 8, pp->pk_seq);
		pvmput16(cp + 10, pp->pk_ack);
		pvmput32(cp + 12, 0);			/* to keep purify happy */
		pvmput8(cp + 12, pp->pk_flag);
#if 0
		/* drop (don't send) random packets */
		if (!(random() & 3)) {
			pvmlogerror("netoutput() darn, dropped one\n");
			cc = -1;
		} else
#endif
			if ((cc = sendto(netsock, cp, len, 0,
					(struct sockaddr*)&hp->hd_sad, sizeof(hp->hd_sad))) == -1
			&& errno != EINTR
#ifndef WIN32
			&& errno != ENOBUFS
#endif
#ifdef IMA_LINUX
			/* some Linux systems report this intermittent error */
			/* && errno != ECONNREFUSED */
#endif
			/* hope this works for all archs, not just linux */
			&& errno != ENOMEM
			) {
				pvmlogperror("netoutput() sendto");
#if defined(IMA_SUN4SOL2) || defined(IMA_X86SOL2) || defined(IMA_SUNMP) || defined(IMA_UXPM) || defined(IMA_UXPV)
	/* life, don't talk to me about life... */
				if (errno == ECHILD)
					pvmlogerror("this message brought to you by solaris\n");
				else
#endif
				pvmbailout(0);
			}
#ifdef	STATISTICS
		if (cc == -1)
			stats.sdneg++;
		else
			stats.sdok++;
#endif

		BCOPY(dummy, cp, sizeof(dummy));	/* restore under header */

	/*
	* set timer for next retry
	*/
		if (cc != -1) {
			if ((pp->pk_flag & (FFFIN|FFACK)) == (FFFIN|FFACK)) {
				pk_free(pp);
				if (hp != hosts->ht_hosts[0]) {
					hostfailentry(hp);
					clear_opq_of((int)(TIDPVMD | hp->hd_hostpart));
					ht_delete(hosts, hp);
					if (newhosts)
						ht_delete(newhosts, hp);
				}
				continue;
			}
			if (!((pp->pk_flag & FFDAT)
					|| (pp->pk_flag & (FFFIN|FFACK)) == FFFIN)) {
				pk_free(pp);
				continue;
			}
			if (!TVISSET(&pp->pk_at))
				pp->pk_at = tnow;
			TVXADDY(&pp->pk_rtv, &tnow, &pp->pk_rta);
			TVXADDY(&pp->pk_rto, &pp->pk_rto, &pp->pk_rta);
#ifdef	STATISTICS
			if (pp->pk_nrt)
				stats.netret++;
#endif
			++pp->pk_nrt;
			if (pp->pk_rta.tv_sec < DDMAXRTT) {
				TVXADDY(&pp->pk_rta, &pp->pk_rta, &pp->pk_rta);
			}

		} else {
			tx.tv_sec = DDERRRETRY/1000000;
			tx.tv_usec = DDERRRETRY%1000000;
			TVXADDY(&pp->pk_rtv, &tnow, &tx);
			TVXADDY(&pp->pk_rto, &pp->pk_rto, &tx);
		}

		/* reinsert packet into opq */

		LISTDELETE(pp, pk_tlink, pk_trlink);
		for (pp2 = opq->pk_trlink; pp2 != opq; pp2 = pp2->pk_trlink)
			if (TVXLTY(&pp2->pk_rtv, &pp->pk_rtv))
				break;
		LISTPUTAFTER(pp2, pp, pk_tlink, pk_trlink);
	}
	return 0;
}
