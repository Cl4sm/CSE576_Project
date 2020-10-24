int
pvm_exit()
{
	int i, lndh;
	int sbf, rbf;
	int cc = 0;
	struct waitc *wp, *wp2;
	struct pmsg *up;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_EXIT,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (pvmmytid != -1) {
		wp = waitlist->wa_link;
		while (wp != waitlist) {
			wp2 = wp->wa_link;
			if (wp->wa_kind == WT_TASKX) {
				up = wp->wa_mesg;
				wp->wa_mesg = 0;
				mesg_input(up);
				LISTDELETE(wp, wa_link, wa_rlink);
			}
			wp = wp2;
		}

		while (pvm_nrecv(-1, -1) > 0) ;		/* XXX attempt to flush messages */

		fflush(stderr);
		fflush(stdout);

		tev_flush( 1 );

		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		if ((cc = msendrecv(TIDPVMD, TM_EXIT, SYSCTX_TM)) > 0) {
			pvm_freebuf(pvm_setrbuf(rbf));
			cc = 0;

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));

		/*
		*  remove all task's message handler functions
		*/
		lndh = ndhandles - 1;
		for (i = lndh; i >= 0; i--){
			pvm_delmhf(i);
		}

		pvmendtask();
	}

	if (TEV_AMEXCL) {
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_exit", cc);
	return cc;
}
