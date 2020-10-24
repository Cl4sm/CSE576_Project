	struct task *tp;
{
	struct pkt *pp = 0;
	struct pkt *pp2;
	int n, m;

again:
	/*
	* if no current packet, start a new one
	*/

	if (!tp->t_rxp) {
		tp->t_rxp = pk_new(pvmudpmtu);
/*
		tp->t_rxp = pk_new(TDFRAGHDR + 2);
*/
		if (DDFRAGHDR > TDFRAGHDR)
			tp->t_rxp->pk_dat += DDFRAGHDR - TDFRAGHDR;
	}
	pp = tp->t_rxp;

	/*
	* read the fragment header and body separately so we can
	* make a bigger buffer if needed
	*/

	n = (pp->pk_len < TDFRAGHDR) ? 0 : pvmget32(pp->pk_dat + 8);
	n += TDFRAGHDR - pp->pk_len;
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("loclinput() t%x fr_len=%d fr_dat=+%d n=%d\n",
				tp->t_tid, pp->pk_len, pp->pk_dat - pp->pk_buf, n);
	}
#ifndef WIN32
	n = read(tp->t_sock, pp->pk_dat + pp->pk_len, n);
#else
	n = win32_read_socket(tp->t_sock,pp->pk_dat + pp->pk_len,n);
#endif
	if (pvmdebmask & PDMPACKET) {
		if (n >= 0) {
			pvmlogprintf("loclinput() read=%d\n", n);
		} else
			pvmlogperror("loclinput() read");
	}

#ifdef	STATISTICS
		switch (n) {
		case -1:
			stats.rdneg++;
			break;
		case 0:
			stats.rdzer++;
			break;
		default:
			stats.rdok++;
			break;
		}
#endif
	if (n == -1) {
		if (errno != EINTR
#ifndef WIN32
				&& errno != EWOULDBLOCK
#endif
		) {
			pvmlogperror("loclinput() read");
			pvmlogprintf("loclinput() marking t%x dead\n",
					tp->t_tid);
			return -1;
		}
		return 0;
	}
	if (!n) {
		if (pvmdebmask & (PDMPACKET|PDMMESSAGE|PDMTASK)) {
			pvmlogprintf("loclinput() read EOF from t%x sock %d\n",
					tp->t_tid, tp->t_sock);
		}
		return -1;
	}

	if ((pp->pk_len += n) < TDFRAGHDR)
		return 0;

	/*
	* if we have a complete frag, accept it
	*/

	m = TDFRAGHDR + pvmget32(pp->pk_dat + 8);
	if (pp->pk_len == m) {
		tp->t_rxp = 0;
		pp->pk_dst = pvmget32(pp->pk_dat);
#if defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_CM5) \
		|| defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
		pp->pk_src = pvmget32(pp->pk_dat + 4);
#else
		pp->pk_src = tp->t_tid;
#endif
		pp->pk_flag = pvmget8(pp->pk_dat + 12);
		pp->pk_len -= TDFRAGHDR;
		pp->pk_dat += TDFRAGHDR;
		if (pp->pk_flag & FFSOM) {
			if (pp->pk_len < MSGHDRLEN) {
				pvmlogprintf(
						"loclinput() SOM pkt src t%x dst t%x too short\n",
						pp->pk_src, pp->pk_dst);
				pk_free(pp);
				return 0;
			}
			pp->pk_enc = pvmget32(pp->pk_dat);
			pp->pk_tag = pvmget32(pp->pk_dat + 4);
			pp->pk_ctx = pvmget32(pp->pk_dat + 8);
			pp->pk_wid = pvmget32(pp->pk_dat + 16);
			pp->pk_crc = pvmget32(pp->pk_dat + 20);
			pp->pk_len -= MSGHDRLEN;
			pp->pk_dat += MSGHDRLEN;
		}
		if (loclinpkt(tp, pp))
			return -1;
		return 0;
	}

	/* realloc buffer if frag won't fit */

	if (pp->pk_len == TDFRAGHDR) {
		if (m > pp->pk_max - (pp->pk_dat - pp->pk_buf)) {
			if (!(tp->t_flag & TF_CONN)) {
				pvmlogprintf(
					"loclinput() unconnected task sends frag length %d (ha)\n",
					m);
				return -1;
			}
			if (DDFRAGHDR > TDFRAGHDR) {
				pp2 = pk_new(m + DDFRAGHDR - TDFRAGHDR);
				pp2->pk_dat += DDFRAGHDR - TDFRAGHDR;
			} else
				pp2 = pk_new(m);
			BCOPY(pp->pk_dat, pp2->pk_dat, TDFRAGHDR);
			pp2->pk_len = pp->pk_len;
			pk_free(pp);
			pp = tp->t_rxp = pp2;
			if (pvmdebmask & PDMPACKET) {
				pvmlogprintf("loclinput() realloc frag max=%d\n", m);
			}
		}
		goto again;
	}

	return 0;
}
