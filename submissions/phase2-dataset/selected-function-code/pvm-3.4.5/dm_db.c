int
dm_db(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int opcode;					/* op requested */
	int tid;
	int req;					/* index requested */
	int flags;
	char *name = 0;				/* class name */
	struct pmsg *mp2 = 0;		/* reply */
	struct pmsg *mp3 = 0;		/* data message */

	struct waitc *wp, *wp2;		/* wait ctx ptrs (notify, recvinfo */
	struct pmsg *mp4 = 0;		/* notify forward message */
	struct hostd *hp2;			/* remote notify host */

	struct pvmmclass *np, *np2;	/* reset pointers */
	struct pvmmentry *ep, *ep2;	/* reset pointers */
	int *noresets;				/* noreset tids */
	int nnr;					/* # of noreset tasks */
	int found;
	int cc;
	int i;
	int notified;

	hp = hp;

	if (upkint(mp, &opcode) || upkint(mp, &tid)
	|| upkstralloc(mp, &name) || upkint(mp, &req) || upkint(mp, &flags))
		goto badformat;

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_DBACK;
	mp2->m_wid = mp->m_wid;

	switch (opcode) {

	case TMDB_PUT:
		mp3 = mesg_new(0);
		if (pmsg_unpack(mp, mp3))
			goto badformat;
		if ((req = mb_insert(tid, name, req, flags, mp3)) < 0)
			pmsg_unref(mp3);
		else {

			/* check for any pending requests for this mbox entry */
			notified = 0;
			for (wp = waitlist->wa_link; wp != waitlist; wp = wp2) {
				wp2 = wp->wa_link;
				if (wp->wa_kind == WT_RECVINFO) {
					ep = (struct pvmmentry *) wp->wa_spec;
					if ( !strcmp( (char *) ep->me_msg, name ) ) {
						cc = mb_lookup(ep->me_tid, (char *) ep->me_msg,
							ep->me_ind, ep->me_flags, &mp3);
						if ( cc != PvmNotFound ) {
							pkint(wp->wa_mesg, cc);
							if (mp3) {
								pmsg_pack(wp->wa_mesg, mp3);
								pmsg_unref(mp3);
							}
							sendmessage(wp->wa_mesg);
							wp->wa_mesg = 0;
							PVM_FREE(ep->me_msg);
							PVM_FREE(ep);
							wait_delete(wp);
						}
					}
				}
				/* check if task needs mbox notify for mb_tidy()... */
				else if (wp->wa_kind == WT_TASKX) {
					if ( wp->wa_on == tid && wp->wa_tid == pvmmytid )
						notified++;
				}
			}

			/* create mbox notify for mb_tidy() cleanup... */
			if ( !notified ) {
				/* dummy notify for clean up */
				wp = wait_new(WT_TASKX);
				wp->wa_on = tid;
				wp->wa_tid = pvmmytid;
				wp->wa_dep = 0;
				wp->wa_mesg = (struct pmsg *) NULL;

				/* pass on to non-master host */
				hp2 = tidtohost(hosts, tid);
				if ( hp2 && hp2->hd_hostpart != myhostpart ) {
					mp4 = mesg_new(0);
					pkint(mp4, PvmTaskExit);
					pkint(mp4, tid);
					mp4->m_dst = hp2->hd_hostpart | TIDPVMD;
					mp4->m_tag = DM_NOTIFY;
					mp4->m_wid = wp->wa_wid;
					sendmessage(mp4);
				}
			}

		}
		pkint(mp2, req);
		break;

	case TMDB_REMOVE:
		req = mb_delete(tid, name, req, flags);
		pkint(mp2, req);
		break;

	case TMDB_GET:
		cc = mb_lookup(tid, name, req, flags, &mp3);
		if ( cc == PvmNotFound && (flags & PvmMboxWaitForInfo) ) {
			ep = me_new(req);
			ep->me_tid = tid;
			ep->me_msg = (struct pmsg *) name;  /* XXX ouch, overload */
			ep->me_flags = flags;

			wp = wait_new(WT_RECVINFO);
			wp->wa_on = tid;
			wp->wa_tid = pvmmytid;
			wp->wa_dep = mp->m_wid;
			wp->wa_mesg = mp2;
			wp->wa_spec = (void *) ep;

			return 0;
		} else {
			pkint(mp2, cc);
			if (mp3) {
				pmsg_pack(mp2, mp3);
				pmsg_unref(mp3);
			}
		}
		break;

	case TMDB_NAMES:
		pkint(mp2, 0);
		req = mb_names(tid, name, mp2);
		break;

	case TMDB_RESET:
		if ( upkint(mp, &nnr) )
			goto badformat;
		noresets = TALLOC( nnr, int, "int" );
		for ( i=0 ; i < nnr ; i++ ) {
			if ( upkint(mp, &(noresets[i])) ) {
				PVM_FREE(noresets);
				goto badformat;
			}
		}
		pkint(mp2, 0);
		for (np = pvmmboxclasses->mc_link; np != pvmmboxclasses;
				np = np2) {
			np2 = np->mc_link;

			/* If name passed in, only wipe mboxes in that class */
			if ( *name == '\0' || !strcmp( np->mc_name, name ) ) {

			for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep2) {
				ep2 = ep->me_link;

				/* If index passed in, only wipe that mbox */
				/* -1 == All Entries */
				if ( req < 0 || req == ep->me_ind ) {

				/* Check for Persistency (that thorn in my "side" :-) */
				if ( ep->me_flags & PvmMboxPersistent ) {

					/* Task Already Gone?  Spank It. */
					if (!(ep->me_tid)) {
						me_free(np, ep);
						if (np2->mc_rlink != np)
							break;
					}
				
					/* Check for No-Reset Task */
					/* (Only if actually killing tasks...) */
					else if ( flags ) {  /* killtasks */
						for ( i=0, found=0 ; i < nnr && !found ; i++ )
							if ( noresets[i] == ep->me_tid )
								found++;

						/* Not a No-Reset, It WILL die soon. */
						/* Wait for cleanup. */
						if ( !found ) {
							wp = wait_new(WT_RESET);
							wp->wa_on = ep->me_tid;
							wp->wa_tid = pvmmytid;
							wp->wa_dep = 0;
							wp->wa_mesg = (struct pmsg *) NULL;

							/* pass on to non-master host */
							hp2 = tidtohost(hosts, ep->me_tid);
							if ( hp2 && hp2->hd_hostpart != myhostpart )
							{
								mp4 = mesg_new(0);
								pkint(mp4, ep->me_tid);
								mp4->m_dst = hp2->hd_hostpart | TIDPVMD;
								mp4->m_tag = DM_RESET;
								mp4->m_wid = wp->wa_wid;
								sendmessage(mp4);
							}
						}
					}
				}
				}
			}
			}
		}
		PVM_FREE(noresets);
		break;

	default:
		goto badformat;
	}

	sendmessage(mp2);
	PVM_FREE(name);
	return 0;

badformat:
	pvmlogerror("dm_db() bad msg format\n");

	if (name)
		PVM_FREE(name);
	if (mp2)
		pmsg_unref(mp2);
	return 0;
}
