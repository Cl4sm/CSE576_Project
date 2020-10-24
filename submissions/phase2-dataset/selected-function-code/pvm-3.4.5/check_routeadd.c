check_routeadd(pcbp)
	struct ttpcb *pcbp;
{
	struct waitc *wp, *wp2;
	struct pmsg *up;
	int sbf;

	wp = waitlist->wa_link;
	while (wp != waitlist) {
		wp2 = wp->wa_link;
		if (wp->wa_kind == WT_ROUTEA) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			pvm_pkint(&pcbp->tt_tid, 1, 1);
			pvm_pkint(&pcbp->tt_fd, 1, 1);
			sbf = pvm_setsbuf(sbf);
			up = midtobuf(sbf);
			up->m_ctx = wp->wa_mesg->m_ctx;
			up->m_tag = wp->wa_mesg->m_tag;
			mesg_input(up);
			if (wp->wa_count != -1 && --wp->wa_count < 1)
				wait_delete(wp);
		}
		wp = wp2;
	}
	return 0;
}
