int
change_trace(tp, trctid, trcctx, trctag)
	struct task *tp;
	int trctid, trcctx, trctag;
{
	struct pmsg *mp;

	if (tp->t_trctid != trctid
			|| tp->t_trcctx != trcctx
			|| tp->t_trctag != trctag) {
		if (tp->t_trctid > 0) {
			tev_send_endtask(
				tp->t_trctid, tp->t_trcctx, tp->t_trctag,
				tp->t_tid, tp->t_status,
				tp->t_utime.tv_sec, tp->t_utime.tv_usec,
				tp->t_stime.tv_sec, tp->t_stime.tv_usec );
		}
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf( "t%x changes trace from <t%x %d %d> to <t%x %d %d>\n",
					tp->t_tid,
					tp->t_trctid, tp->t_trcctx, tp->t_trctag,
					trctid, trcctx, trctag);
		}
		tp->t_trctid = trctid;
		tp->t_trcctx = trcctx;
		tp->t_trctag = trctag;
		if (tp->t_trctid > 0) {
			tev_send_spntask(
				tp->t_trctid, tp->t_trcctx, tp->t_trctag,
				tp->t_tid, tp->t_ptid );
			tev_send_newtask(
				tp->t_trctid, tp->t_trcctx, tp->t_trctag,
				tp->t_tid, tp->t_ptid, 0 /* XXX lie */,
				tp->t_a_out ? tp->t_a_out : "" );
		}
	}
	return 0;
}
