task_new(tid)
	int tid;
{
	struct task *tp, *tp2;

	if (tid) {
		ntasks++;
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("task_new(%d) ntasks=%d pvm_max_ntasks=%d\n",
				tid, ntasks, pvm_max_ntasks );
		}
		if (pvm_max_ntasks && ntasks > pvm_max_ntasks) {
			pvmlogprintf("task_new() too many tasks?\n");
			ntasks--;  /* this one didn't make it... :-) */
			return( (struct task *) NULL );
		}
	}

	if (!(tp = TALLOC(1, struct task, "task"))) {
		pvmlogprintf("task_new() can't get memory\n");
		pvmbailout(0);
	}
	BZERO((char*)tp, sizeof(struct task));
	tp->t_tid = tid;
	tp->t_txq = pk_new(0);
	tp->t_sock = -1;
	tp->t_out = -1;
	tp->t_authfd = -1;
	tp->t_schedlmsg = 0;
	tp->t_ccs = TALLOC(1, struct ccon, "ccon");
    tp->t_ccs->c_link = tp->t_ccs->c_rlink = 0;
	tp->t_ccs->c_peer = tp->t_ccs->c_rpeer = tp->t_ccs;
    tp->t_ccs->c_cid = 0;
    tp->t_ccs->c_tid = tid;
	FORLIST (tp2, locltasks, t_link)
		if (tp2->t_tid > tid)
			break;
	LISTPUTBEFORE(tp2, tp, t_link, t_rlink);
	return tp;
}
