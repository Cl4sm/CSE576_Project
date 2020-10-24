change_output(tp, outtid, outctx, outtag)
	struct task *tp;
	int outtid, outctx, outtag;
{
	struct pmsg *mp;

	if (tp->t_outtid != outtid
	|| tp->t_outctx != outctx
	|| tp->t_outtag != outtag) {
		if (tp->t_outtid > 0) {
			mp = mesg_new(0);
			mp->m_dst = tp->t_outtid;
			mp->m_ctx = tp->t_outctx;
			mp->m_tag = tp->t_outtag;
			pkint(mp, tp->t_tid);
			pkint(mp, TO_EOF);
			sendmessage(mp);
		}
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("t%x changes output from <t%x %d %d> to <t%x %d %d>\n",
					tp->t_tid,
					tp->t_outtid, tp->t_outctx, tp->t_outtag,
					outtid, outctx, outtag);
		}
		tp->t_outtid = outtid;
		tp->t_outctx = outctx;
		tp->t_outtag = outtag;
		if (tp->t_outtid > 0) {
			mp = mesg_new(0);
			mp->m_dst = tp->t_outtid;
			mp->m_ctx = tp->t_outctx;
			mp->m_tag = tp->t_outtag;
			pkint(mp, tp->t_tid);
			pkint(mp, TO_SPAWN);
			pkint(mp, tp->t_ptid);
			sendmessage(mp);

			mp = mesg_new(0);
			mp->m_dst = tp->t_outtid;
			mp->m_ctx = tp->t_outctx;
			mp->m_tag = tp->t_outtag;
			pkint(mp, tp->t_tid);
			pkint(mp, TO_NEW);
			pkint(mp, tp->t_ptid);
			sendmessage(mp);
		}
	}
	return 0;
}
