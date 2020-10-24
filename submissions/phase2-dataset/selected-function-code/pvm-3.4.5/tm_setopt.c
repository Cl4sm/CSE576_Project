int
tm_setopt(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int what;
	char *val;
	int x;

	while (!upkint(mp, &what) && !upkstralloc(mp, &val)) {
		switch (what) {

		case TS_OUTTID:
			x = pvmxtoi(val);
			change_output(tp, x, tp->t_outctx, tp->t_outtag);
			break;

		case TS_OUTCTX:
			x = pvmxtoi(val);
			change_output(tp, tp->t_outtid, x, tp->t_outtag);
			break;

		case TS_OUTTAG:
			x = pvmxtoi(val);
			change_output(tp, tp->t_outtid, tp->t_outctx, x);
			break;

		case TS_TRCTID:
			x = pvmxtoi(val);
			change_trace(tp, x, tp->t_trcctx, tp->t_trctag);
			break;

		case TS_TRCCTX:
			x = pvmxtoi(val);
			change_trace(tp, tp->t_trctid, x, tp->t_trctag);
			break;

		case TS_TRCTAG:
			x = pvmxtoi(val);
			change_trace(tp, tp->t_trctid, tp->t_trcctx, x);
			break;

		default:
			pvmlogprintf("tm_setopt() ? option %d val <%s>\n", what, val);
			break;
		}
		if (val)
			PVM_FREE(val);
	}
	mp = replymessage(mp);
	sendmessage(mp);
	return 0;
}
