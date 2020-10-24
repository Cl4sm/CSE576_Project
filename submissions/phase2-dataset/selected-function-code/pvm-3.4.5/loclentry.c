loclentry(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int c = mp->m_tag;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("loclentry() from t%x tag %s\n", mp->m_src,
				pvmnametag(c, (int *)0));
/*
		pvmhdump(mp->m_cfrag->fr_dat, mp->m_cfrag->fr_len, "frag: ");
*/
	}

/*
	if (mp->m_enc != 1) {
		pvmlogprintf("loclentry() message from t%x with bad enc %d\n",
				tp->t_tid, mp->m_enc);
		goto bail;
	}
*/

	if (c < (int)TM_FIRST || c > (int)TM_LAST) {
		pvmlogprintf("loclentry() message from t%x with bogus tag %d\n",
				tp->t_tid, c);
		goto bail;
	}

	if ((!(tp->t_flag & TF_CONN) && c != TM_CONNECT && c != TM_CONN2 
								 && c != TM_SHMCONN)
	|| ((tp->t_flag & TF_AUTH) && c != TM_CONN2)) {
		pvmlogerror("loclentry() non-connect message from anon task\n");
		tp->t_flag |= TF_CLOSE;
		goto bail;
	}

	c -= TM_FIRST;
	(loclswitch[c])(tp, mp);

bail:
	pmsg_unref(mp);
	return 0;
}
