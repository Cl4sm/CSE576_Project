int
tm_notify(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int what, flags, ctx, tag, count, tid;
	struct hostd *hp;
	struct pmsg *mp2;
	struct waitc *wp;
	int i;

	if (upkint(mp, &what)
	|| upkint(mp, &ctx)
	|| upkint(mp, &tag)
	|| upkint(mp, &count)) {
		pvmlogerror("tm_notify() bad msg format\n");
		return 0;
	}

	flags = what;
	what &= 0xff;
	switch (what) {

	/*
	* TaskExit: make a wait context that hangs out until the task is
	* cleaned up.
	*/

	case PvmTaskExit:
		for (i = 0; i < count; i++) {
			if (upkuint(mp, &tid)) {
				pvmlogerror("tm_notify() bad msg format\n");
				return 0;
			}

			if (flags & PvmNotifyCancel) {
				FORLIST (wp, waitlist, wa_link)
					if (wp->wa_kind == WT_TASKX
					&& wp->wa_on == tid
					&& wp->wa_tid == tp->t_tid
					&& wp->wa_mesg->m_ctx == ctx
					&& wp->wa_mesg->m_tag == tag)
						break;
				if (wp != waitlist) {
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
					if ((hp = tidtohost(hosts, tid))
					&& hp->hd_hostpart != myhostpart)
						wp->wa_tid = 0;
					else
						wait_delete(wp);
				}

			} else {
				/* make reply message */

				mp2 = mesg_new(0);
				mp2->m_dst = tp->t_tid;
				mp2->m_ctx = ctx;
				mp2->m_tag = tag;
				mp2->m_wid = mp->m_wid;
				pkint(mp2, tid);

				/* if task doesn't exist, reply immediately */

				if (!(hp = tidtohost(hosts, tid))
				|| (hp->hd_hostpart == myhostpart && !task_find(tid)))
					sendmessage(mp2);

				else {

					/* otherwise make a wc for it */

					wp = wait_new(WT_TASKX);
					wp->wa_on = tid;
					wp->wa_tid = tp->t_tid;
					wp->wa_dep = mp->m_wid;
					wp->wa_mesg = mp2;

					/* and if not on this host, pass on the request */

					if (hp->hd_hostpart != myhostpart) {
						mp2 = mesg_new(0);
						pkint(mp2, what);
						pkint(mp2, tid);
						mp2->m_dst = hp->hd_hostpart | TIDPVMD;
						mp2->m_tag = DM_NOTIFY;
						mp2->m_wid = wp->wa_wid;
						sendmessage(mp2);
					}
				}
			}
		}
		break;

	case PvmHostDelete:
		for (i = 0; i < count; i++) {
			if (upkuint(mp, &tid)) {
				pvmlogerror("tm_notify() bad msg format\n");
				return 0;
			}
			if (flags & PvmNotifyCancel) {
				if (hp = tidtohost(hosts, tid)) {
					FORLIST (wp, waitlist, wa_link)
						if (wp->wa_kind == WT_HOSTF
						&& wp->wa_on == hp->hd_hostpart
						&& wp->wa_tid == tp->t_tid
						&& wp->wa_mesg->m_ctx == ctx
						&& wp->wa_mesg->m_tag == tag)
							break;
					if (wp != waitlist) {
						sendmessage(wp->wa_mesg);
						wp->wa_mesg = 0;
						wait_delete(wp);
					}
				}

			} else {
				mp2 = mesg_new(0);
				mp2->m_dst = mp->m_src;
				mp2->m_ctx = ctx;
				mp2->m_tag = tag;
				pkint(mp2, tid);
				if (hp = tidtohost(hosts, tid)) {
					wp = wait_new(WT_HOSTF);
					wp->wa_tid = tp->t_tid;
					wp->wa_on = hp->hd_hostpart;
					wp->wa_mesg = mp2;

				} else {
					sendmessage(mp2);
				}
			}
		}
		break;

	case PvmHostAdd:

		FORLIST (wp, waitlist, wa_link)
			if (wp->wa_kind == WT_HOSTA
			&& wp->wa_tid == tp->t_tid
			&& wp->wa_mesg->m_ctx == ctx
			&& wp->wa_mesg->m_tag == tag)
				break;

	/* if cancelling, delete possible existing waitc */

		if (count == 0 || (flags & PvmNotifyCancel)) {
			if (wp != waitlist)
				wait_delete(wp);

	/* otherwise, update existing waitc or create new one */
		} else {
			if (wp == waitlist) {
				wp = wait_new(WT_HOSTA);
				wp->wa_tid = tp->t_tid;
				wp->wa_on = tp->t_tid;
				wp->wa_mesg = mesg_new(0);
				wp->wa_mesg->m_dst = tp->t_tid;
				wp->wa_mesg->m_ctx = ctx;
				wp->wa_mesg->m_tag = tag;
			}
			wp->wa_count = count;
		}
		break;

	default:
		pvmlogprintf("tm_notify() unknown what=%d\n", what);
		break;
	}
	return 0;
}
