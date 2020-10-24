int
tm_tracer(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int regme;
	int tctx;
	int ttag;
	int octx;
	int otag;
	Pvmtmask tmask;
	int tbuf;
	int topt;
	struct pmsg *mp2;
	struct pmsg *mpd;
	int slconf;
	int hh;
	struct hostd *hp;
	char buf[512];

	if (upkint(mp, &regme)) {
		pvmlogerror("tm_tracer() bad msg format\n");
		return 0;
	}

	mp2 = replymessage(mp);

	slconf = 0;

	if (regme) {
		if (pvmtracer.trctid) {
			pkint(mp2, PvmAlready);

		} else {
			tp->t_flag |= TF_ISTRACER;
			upkint(mp, &tctx);
			upkint(mp, &ttag);
			upkint(mp, &octx);
			upkint(mp, &otag);
			upkstr(mp, tmask, TEV_MASK_LENGTH);
			upkint(mp, &tbuf);
			upkint(mp, &topt);
			pvmtracer.trctid = tp->t_tid;
			pvmtracer.trcctx = tctx;
			pvmtracer.trctag = ttag;
			pvmtracer.outtid = tp->t_tid;
			pvmtracer.outctx = octx;
			pvmtracer.outtag = otag;
			BCOPY(tmask,pvmtracer.tmask,TEV_MASK_LENGTH);
			pvmtracer.trcbuf = tbuf;
			pvmtracer.trcopt = topt;
			slconf++;
			if (pvmdebmask & PDMTRACE) {
				pvmlogprintf("tm_tracer() register t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);
		}

	} else {
		if (pvmtracer.trctid == tp->t_tid) {
			tp->t_flag &= ~TF_ISTRACER;
			pvmtracer.trctid = 0;
			pvmtracer.trcctx = 0;
			pvmtracer.trctag = 0;
			pvmtracer.outtid = 0;
			pvmtracer.outctx = 0;
			pvmtracer.outtag = 0;
			TEV_MASK_INIT(pvmtracer.tmask);
			pvmtracer.trcbuf = 0;
			pvmtracer.trcopt = 0;
			slconf++;
			if (pvmdebmask & PDMTRACE) {
				pvmlogprintf("tm_tracer() unregister t%x \n",
					tp->t_tid);
			}
			pkint(mp2, 0);

		} else {
			if (pvmdebmask & PDMTRACE) {
				pvmlogprintf("tm_tracer() t%x tries to unregister?\n",
						tp->t_tid);
			}
			pkint(mp2, PvmNoTask);
		}
	}

	/* Update slave pvmds */
	if ( slconf ) {
		for (hh = hosts->ht_last; hh > 0; hh--) {
			if (hp = hosts->ht_hosts[hh]) {
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

	/* Send response to requesting task */
	sendmessage(mp2);

	return 0;
}
