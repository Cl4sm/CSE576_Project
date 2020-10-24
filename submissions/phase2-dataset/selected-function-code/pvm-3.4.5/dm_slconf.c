dm_slconf(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int t;				/* field type */
	char *s, *s2;

	mp = mp;
	hp = hp;

	while (!upkint(mp, &t) && !upkstralloc(mp, &s)) {
		switch (t) {

		case DM_SLCONF_EP:
			if (pvmdebmask & (PDMTASK|PDMSTARTUP)) {
				pvmlogprintf("dm_slconf() ep<%s>\n", s);
			}
			epaths = colonsep(varsub(s));
			PVM_FREE(s);
			break;

		case DM_SLCONF_BP:
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("dm_slconf() bp<%s>\n", s);
			}
			debugger = varsub(s);
			PVM_FREE(s);
			break;

		case DM_SLCONF_WD:
			if (pvmdebmask & (PDMTASK|PDMSTARTUP)) {
				pvmlogprintf("dm_slconf() wd<%s>\n", s);
			}
			s2 = varsub(s);
			if (chdir(s2) == -1)
				pvmlogperror(s2);
			PVM_FREE(s);
			PVM_FREE(s2);
			break;

		case DM_SLCONF_SCHED:
			if (pvmdebmask & (PDMSCHED|PDMSTARTUP)) {
				pvmlogprintf("dm_slconf() sched<t%x>\n", pvmschedtid);
			}
			pvmschedtid = pvmxtoi(s);
			break;

		case DM_SLCONF_TRACE: {
			Pvmtmask tmask;
			int ttid, tctx, ttag, otid, octx, otag, tbuf, topt;
			if (pvmdebmask & (PDMTRACE|PDMSTARTUP)) {
				pvmlogprintf("dm_slconf() tracer<t%x>\n",
					pvmtracer.trctid);
			}
			if (sscanf(s, "%x %d %d %x %d %d %d %d %s",
					&ttid, &tctx, &ttag, &otid, &octx, &otag,
					&tbuf, &topt, tmask) != 9) {
				pvmlogprintf("dm_slconf() bogus string<%s>\n", s);
			}
			else {
				pvmtracer.trctid = ttid;
				pvmtracer.trcctx = tctx;
				pvmtracer.trctag = ttag;
				pvmtracer.outtid = otid;
				pvmtracer.outctx = octx;
				pvmtracer.outtag = otag;
				pvmtracer.trcbuf = tbuf;
				pvmtracer.trcopt = topt;
				BCOPY(tmask,pvmtracer.tmask,TEV_MASK_LENGTH);
			}
			break;
		}

		default:
			pvmlogprintf("dm_slconf() ? type %d val <%s>\n", t, s);
			PVM_FREE(s);
			break;
		}
	}
	return 0;
}
