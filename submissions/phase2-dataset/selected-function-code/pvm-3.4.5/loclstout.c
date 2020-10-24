	struct task *tp;
{
	int n;
	struct pmsg *mp;

	static char buf[4000];

#ifndef WIN32
	n = read(tp->t_out, buf, sizeof(buf) - 1);
#else
	n = win32_read_socket(tp->t_out,buf, sizeof(buf) -1);
#endif

	if (n < 1) {
		if (n == 0 || (errno != EINTR
#ifndef WIN32
				&& errno != EWOULDBLOCK
#endif
			)) {
			wrk_fds_delete(tp->t_out, 1);
#ifndef WIN32
			(void)close(tp->t_out);
#else
			(void)_close(tp->t_out);
#endif
			tp->t_out = -1;
			if (tp->t_outtid > 0) {
				mp = mesg_new(0);
				mp->m_dst = tp->t_outtid;
				mp->m_ctx = tp->t_outctx;
				mp->m_tag = tp->t_outtag;
				pkint(mp, tp->t_tid);
				pkint(mp, TO_EOF);
				sendmessage(mp);
				tp->t_outtid = 0;
			}
		}

	} else {
		mp = mesg_new(0);
		pkint(mp, tp->t_tid);
		pkint(mp, n);
		pkbyte(mp, buf, n);
		if (tp->t_outtid > 0) {
			mp->m_dst = tp->t_outtid;
			mp->m_ctx = tp->t_outctx;
			mp->m_tag = tp->t_outtag;

		} else {
			mp->m_tag = DM_TASKOUT;
			mp->m_dst = hosts->ht_hosts[hosts->ht_cons]->hd_hostpart | TIDPVMD;
		}
		sendmessage(mp);
	}
	return 0;
}
