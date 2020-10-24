	struct task *tp;
{
	struct pkt *pp;
	char *cp;
	int len;
	int n;

	while ((pp = tp->t_txq->pk_link)->pk_buf) {

		if (!pp->pk_cpos || pp->pk_cpos < pp->pk_dat) {
	/*
	* prepend frag [message] headers if we'll be writing them.
	*/
			cp = pp->pk_dat;
			len = pp->pk_len;
			if (pp->pk_flag & FFSOM) {
				cp -= MSGHDRLEN;
				len += MSGHDRLEN;
				if (cp < pp->pk_buf) {
					pvmlogerror("locloutput() no headroom for message header\n");
					return 0;
				}
				pvmput32(cp, pp->pk_enc);
				pvmput32(cp + 4, pp->pk_tag);
				pvmput32(cp + 8, pp->pk_ctx);
				pvmput32(cp + 16, pp->pk_wid);
				pvmput32(cp + 20, pp->pk_crc);
			}
			cp -= TDFRAGHDR;
			if (cp < pp->pk_buf) {
				pvmlogerror("locloutput() no headroom for packet header\n");
				return 0;
			}
			pvmput32(cp, pp->pk_dst);
			pvmput32(cp + 4, pp->pk_src);
			pvmput32(cp + 8, len);
			pvmput32(cp + 12, 0);			/* to keep purify happy */
			pvmput8(cp + 12, pp->pk_flag & (FFSOM|FFEOM));
			len += TDFRAGHDR;
		}

		if (pp->pk_cpos) {
			cp = pp->pk_cpos;
			len = pp->pk_len + (pp->pk_dat - cp);

		} else {
			pp->pk_cpos = cp;
			if (pvmdebmask & PDMPACKET) {
				pvmlogprintf(
					"locloutput() src t%x dst t%x f %s len %d\n",
					pp->pk_src, pp->pk_dst, pkt_flags(pp->pk_flag), len);
			}
		}

	/*
	* send as much as possible; skip to next packet when all sent
	*/

#if defined(IMA_RS6K) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
		n = write(tp->t_sock, cp, min(len, 4096));
#else
#ifndef WIN32
		n = write(tp->t_sock, cp, len);
#else
	 	n = win32_write_socket(tp->t_sock,cp,len);
#endif
#endif

#ifdef	STATISTICS
		if (n == -1)
			stats.wrneg++;
		else
			if (!n)
				stats.wrzer++;
			else
				if (n == len)
					stats.wrok++;
				else
					stats.wrshr++;
#endif
		if (n == -1) {
			if (errno != EINTR
#ifndef WIN32
					&& errno != EWOULDBLOCK
					&& errno != ENOBUFS
#endif
					&& errno != EAGAIN)
			{
#ifdef WIN32
			if (GetLastError() != WSAECONNRESET) {
#endif
				pvmlogperror("locloutput() write");
				pvmlogprintf("locloutput() marking t%x dead\n",
						tp->t_tid);
#ifdef WIN32
			}
#endif
				return -1;
			}
			break;
		}

		if (n > 0) {
			if (pvmdebmask & PDMPACKET) {
				pvmlogprintf(
						"locloutput() src t%x dst t%x wrote %d\n",
						pp->pk_src, pp->pk_dst, n);
			}
			if ((len - n) > 0) {
				pp->pk_cpos += n;

			} else {
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
				int dst = pp->pk_dst;
#endif
				LISTDELETE(pp, pk_link, pk_rlink);
				pk_free(pp);
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2)
				/* Not done for BEOLIN, since tp2 is tp, */
				/* the task on which we are working (PLS) */
				if (TIDISNODE(dst)) {
					struct task *tp2;

					/* Expensive! But what else can we do? */
					if ((tp2 = task_find(dst)) && (tp2->t_flag & TF_CLOSE)) {
						mpp_free(tp2);
						/* XXX task_cleanup(tp2); */
						task_free(tp2);
					}
				}
#endif /*defined(IMA_CM5) || defined(IMA_SP2MPI)*/
			}

		} else
			break;
	}

	if (tp->t_txq->pk_link == tp->t_txq) {
		wrk_fds_delete(tp->t_sock, 2);

	/* flush context if TF_CLOSE set */

		if (tp->t_flag & TF_CLOSE)
			return -1;
	}

	return 0;
}
