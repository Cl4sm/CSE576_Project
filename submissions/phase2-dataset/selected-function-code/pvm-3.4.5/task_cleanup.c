task_cleanup(tp)
	struct task *tp;
{
	struct pmsg *mp;
	struct waitc *wp, *wp2;
	struct task *tp2;
	struct pvmmentry *ep;
	struct hostd *hp;
	struct pmsg *mpd;
	char buf[512];
	int hh;

	/* notify anyone who asked */

	if (tp->t_tid) {
		if (pvmdebmask & PDMTASK)
			pvmlogprintf("task_cleanup() t%x\n", tp->t_tid);

		for (wp = waitlist->wa_link; wp != waitlist; wp = wp->wa_link) {

	/* waits depending on this task */

			if (wp->wa_on == tp->t_tid) {
				switch (wp->wa_kind) {

				case WT_HOSTSTART:
					busyadding = 0;
					free_waitc_add((struct waitc_add *)wp->wa_spec);
					pkint(wp->wa_mesg, PvmDSysErr);
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
					if (pvmdebmask & (PDMTASK|PDMSTARTUP))
						pvmlogprintf(
							"task_cleanup() hoster t%x takes wid %d with it\n",
							tp->t_tid, wp->wa_wid);
					break;

				case WT_TASKSTART:
					if (wp->wa_tid) {
						if (pvmdebmask & PDMTASK) {
							pvmlogprintf(
								"task_cleanup() tasker t%x takes t%x with it\n",
								tp->t_tid, wp->wa_tid);
						}
						if (tp2 = task_find(wp->wa_tid)) {
							wp->wa_tid = 0;
							task_cleanup(tp2);
							task_free(tp2);
						}
					}
					break;

				case WT_TASKX:
					if (wp->wa_tid && wp->wa_mesg) {
						sendmessage(wp->wa_mesg);
						wp->wa_mesg = 0;
					}
					mb_tidy(tp->t_tid);
					break;

				case WT_RESET:
					if (wp->wa_tid && wp->wa_mesg) {
						sendmessage(wp->wa_mesg);
						wp->wa_mesg = 0;
					}
					mb_tidy_reset(tp->t_tid);
					break;

				case WT_RECVINFO:
					/* clean up pending recvinfo */
					ep = (struct pvmmentry *) wp->wa_spec;
					if ( ep->me_msg )	/* class name (overload :-Q) */
						PVM_FREE( ep->me_msg );
					PVM_FREE( ep );
					break;

				case WT_HOSTA:
					break;

				default:
					pvmlogprintf(
							"task_cleanup() can't deal with wait kind %d\n",
							wp->wa_kind);
					break;
				}
				wp2 = wp;
				wp = wp->wa_rlink;
				wait_delete(wp2);
				continue;
			}

	/* waits this task was waiting on */

			if (wp->wa_tid == tp->t_tid) {
				switch (wp->wa_kind) {

				case WT_HOSTF:
				case WT_HOSTA:
				case WT_TASKX:
				case WT_ROUTEA:
				case WT_ROUTED:
				case WT_TASKSTART:
					wp2 = wp;				/* some kinds we can toss now */
					wp = wp->wa_rlink;
					wait_delete(wp2);
					break;

				default:
					wp->wa_tid = 0;			/* in case tid gets recycled */
					break;
				}
			}
		}

		/* notify the scheduler */

		if ((tp->t_sched)&&(tp->t_schedlmsg!=SM_TASKX)) {
			mp = mesg_new(0);
			mp->m_dst = tp->t_sched;
			mp->m_tag = SM_TASKX;
			tp->t_schedlmsg = SM_TASKX;
			if (pvmdebmask & PDMSCHED) {
				pvmlogprintf("task_cleanup() taskx to t%x status = 0x%x\n",
						tp->t_sched, tp->t_status);
			}
			pkint(mp, tp->t_tid);
			pkint(mp, tp->t_status);
			pkint(mp, (int)tp->t_utime.tv_sec);
			pkint(mp, (int)tp->t_utime.tv_usec);
			pkint(mp, (int)tp->t_stime.tv_sec);
			pkint(mp, (int)tp->t_stime.tv_usec);
			sendmessage(mp);
		}

		/* check if it's the hoster */

		if (tp->t_tid == hostertid) {
			if (pvmdebmask & (PDMTASK|PDMSTARTUP)) {
				pvmlogprintf("task_cleanup() unreg hoster t%x\n", tp->t_tid);
			}
			hostertid = 0;
		}

		/* check if it's the tasker */

		if (tp->t_tid == taskertid) {
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("task_cleanup() unreg tasker t%x\n", tp->t_tid);
			}
			taskertid = 0;
		}

		/* check if it's the tracer */

		if (tp->t_tid == pvmtracer.trctid) {

			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("task_cleanup() unreg tracer t%x\n",
						tp->t_tid);
			}

			pvmtracer.trctid = 0;
			pvmtracer.trcctx = 0;
			pvmtracer.trctag = 0;
			pvmtracer.outtid = 0;
			pvmtracer.outctx = 0;
			pvmtracer.outtag = 0;
			TEV_MASK_INIT(pvmtracer.tmask);
			pvmtracer.trcbuf = 0;
			pvmtracer.trcopt = 0;

			/* tell the other pvmds */

			for (hh = hosts->ht_last; hh > 0; hh--) {
				if (hh != hosts->ht_local
						&& (hp = hosts->ht_hosts[hh])) {
					mpd = mesg_new(0);
					mpd->m_tag = DM_SLCONF;
					mpd->m_dst = hp->hd_hostpart | TIDPVMD;
					pkint(mpd, DM_SLCONF_TRACE);
					sprintf(buf, "%x %d %d %x %d %d %d %d %s",
						pvmtracer.trctid, pvmtracer.trcctx,
							pvmtracer.trctag,
						pvmtracer.outtid, pvmtracer.outctx,
							pvmtracer.outtag,
						pvmtracer.trcbuf, pvmtracer.trcopt,
						pvmtracer.tmask);
					pkstr(mpd, buf);
					sendmessage(mpd);
				}
			}
		}
	}

	/* complete multicast */

	if (tp->t_mca) {
/* XXX should send an EOM frag to all rcpts */
		mca_free(tp->t_mca);
		tp->t_mca = 0;
	}
}
