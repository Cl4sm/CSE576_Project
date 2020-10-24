void
task_dump()
{
	struct task *tp;
	struct pkt *pp;
	struct ccon *cp;

	pvmlogprintf("task_dump()\n");
	pvmlogprintf("     tid     ptid flag    pid soc out     wait   outtid   trctid    sched   es\n");
	for (tp = locltasks->t_link; tp != locltasks; tp = tp->t_link) {
		pvmlogprintf("%8x %8x %4x %6d %3d %3d %8d %8x %8x %8x %4x\n",
				tp->t_tid,
				tp->t_ptid,
				tp->t_flag,
				tp->t_pid,
				tp->t_sock,
				tp->t_out,
				(tp->t_wait ? tp->t_wait->wa_wid : 0),
				tp->t_outtid,
				tp->t_trctid,
				tp->t_sched,
				tp->t_status);
		if (LISTFIRST(tp->t_txq, pk_link)) {
			pvmlogprintf(" txq:pkt      src      dst flag    len    ofs\n");
			FORLIST (pp, tp->t_txq, pk_link) {
				pvmlogprintf("%08x %8x %8x %4x %6d %6d\n",
						pp,
						pp->pk_src,
						pp->pk_dst,
						pp->pk_flag,
						pp->pk_len,
						pp->pk_dat - pp->pk_buf);
			}
		}
		if (LISTFIRST(tp->t_ccs, c_peer)) {
			pvmlogprintf("contexts:");
			FORLIST (cp, tp->t_ccs, c_peer)
				pvmlogprintf(" 0x%x", cp->c_cid);
			pvmlogprintf("\n");
		}
	}
}
