int
check_routedelete(pcbp)
	struct ttpcb *pcbp;
{
	struct waitc *wp, *wp2;
	struct pmsg *up;
	int tid = pcbp->tt_tid;

	wp = waitlist->wa_link;
	while (wp != waitlist) {
		wp2 = wp->wa_link;
		if (wp->wa_kind == WT_ROUTED && tid == wp->wa_on) {
			up = wp->wa_mesg;
			wp->wa_mesg = 0;
			mesg_input(up);
			wait_delete(wp);
		}
		wp = wp2;
	}
	return 0;
}
