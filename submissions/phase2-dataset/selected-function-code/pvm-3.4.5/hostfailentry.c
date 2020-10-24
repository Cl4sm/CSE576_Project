hostfailentry(hp)
	struct hostd *hp;
{
	int hpart = hp->hd_hostpart;
	struct waitc *wp, *wp2;
	struct pmsg *mp;
	struct pvmmentry *ep;

	if (pvmdebmask & PDMHOST) {
		pvmlogprintf("hostfailentry() host %s\n", hp->hd_name);
		hd_dump(hp);
	}

	if (hp == hosts->ht_hosts[hosts->ht_master]) {
		pvmlogerror("hostfailentry() lost master host, we're screwwwed\n");
		pvmbailout(0);
	}

	/*
	* if we're master pvmd, send HT_DEL message to all others
	*/

	if (hp->hd_hostpart && hosts->ht_master == hosts->ht_local) {
		struct hostd *hp2;
		int hh;

		mp = mesg_new(0);
		mp->m_tag = DM_HTDEL;
		pkint(mp, hosts->ht_serial);
		pkint(mp, hp->hd_hostpart);
		for (hh = hosts->ht_last; hh > 0; hh--)
			if (hh != hosts->ht_local
			&& (hp2 = hosts->ht_hosts[hh]) && hp2 != hp) {
				mp->m_ref++;
				mp->m_dst = hp2->hd_hostpart | TIDPVMD;
				sendmessage(mp);
			}
		pmsg_unref(mp);

		/* inform the scheduler too */

		if (pvmschedtid) {
			mp = mesg_new(0);
			mp->m_tag = SM_HOSTX;
			mp->m_dst = pvmschedtid;
			pkint(mp, hp->hd_hostpart | TIDPVMD);
			sendmessage(mp);
		}
	}

	for (wp = waitlist->wa_link; wp != waitlist; wp = wp->wa_link) {
		if (wp->wa_on && (wp->wa_on & TIDHOST) == hpart) {
			switch (wp->wa_kind) {

			case WT_ADDHOST:	/* the master must have died */
			case WT_DELHOST:	/* the master must have died */

				pvmlogprintf("hostfailentry() can't deal with wait kind %d\n",
						wp->wa_kind);
				break;

			case WT_HTUPD:
				if (wp->wa_peer == wp) {
					int hh;

					mp = mesg_new(0);
					mp->m_tag = DM_HTCOMMIT;

					for (hh = hosts->ht_last; hh > 0; hh--)
						if (hh != hosts->ht_local && (hp = hosts->ht_hosts[hh])) {
							mp->m_ref++;
							mp->m_dst = hp->hd_hostpart | TIDPVMD;
							sendmessage(mp);
						}
					pmsg_unref(mp);

					busyadding = 0;
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
				}
				break;

			case WT_SPAWN:
				{
					struct waitc_spawn *wxp;
					int retry = 0;
					int v;

					wxp = (struct waitc_spawn*)wp->wa_spec;

	/* mark tasks assigned to this host as failed */
	/* (or reset if PvmTaskDefault) */

					for (v = wxp->w_veclen; v-- > 0; )
						if (wxp->w_vec[v] == hp->hd_hostpart)
							if (!(wxp->w_flags
									& (PvmTaskHost|PvmTaskArch)))
							{
								wxp->w_vec[v] = 0;
								retry++;
							}
							else
								wxp->w_vec[v] = PvmHostFail;

					ht_delete(wxp->w_ht, hp);

	/* try assigning task again without failed host */

					if ( retry )
						assign_tasks(wp);

	/* is this was the last wait, reply to task */

					if (wp->wa_peer == wp)
						assign_tasks(wp);
				}
				break;

			case WT_TASK:

	/* send message if we're the last waiter */

				if (wp->wa_peer == wp) {
					mp = wp->wa_mesg;
					mp->m_ref++;
					sendmessage(mp);
				}
				break;

			case WT_HOSTSTART:

	/* reply to waiter */

				busyadding = 0;
				if (wp->wa_spec) {
					free_waitc_add((struct waitc_add *)wp->wa_spec);
					wp->wa_spec = 0;
				}
				pkint(wp->wa_mesg, PvmDSysErr);
				sendmessage(wp->wa_mesg);
				wp->wa_mesg = 0;
				break;

			case WT_TASKX:
				if (wp->wa_tid && wp->wa_mesg) {
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
				}
				mb_tidy(wp->wa_on);
				break;

			case WT_RESET:
				if (wp->wa_tid && wp->wa_mesg) {
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
				}
				mb_tidy_reset(wp->wa_on);
				break;

			case WT_RECVINFO:
				/* clean up pending recvinfo */
				ep = (struct pvmmentry *) wp->wa_spec;
				if ( ep->me_msg )	/* class name (overload :-Q) */
					PVM_FREE( ep->me_msg );
				PVM_FREE( ep );
				break;

			case WT_HOSTF:
				sendmessage(wp->wa_mesg);
				wp->wa_mesg = 0;
				break;

			case WT_PSTAT:
			case WT_MSTAT:
			case WT_HOSTSYNC:
				pkint(wp->wa_mesg, PvmHostFail);
				sendmessage(wp->wa_mesg);
				wp->wa_mesg = 0;
				break;

			default:
				pvmlogprintf("hostfailentry() alien wait kind %d\n",
						wp->wa_kind);
				break;
			}

			wp2 = wp->wa_rlink;
			wait_delete(wp);
			wp = wp2;
		}
	}
	return 0;
}
