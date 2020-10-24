int
ent_add(register u_int32_t a, register u_char *e, time_t t, register char *h,
	char *interface)
{
	register struct ainfo *ap;
	register struct einfo *ep;
	register int i;
	register u_int len;
	u_char *e2;
	time_t t2;

	/* Ignore 0.0.0.0 */
	if (a == 0) {
		return(1);
	}

	/* Lookup ip address */
	ap = ainfo_find(a);

	/* Check for the usual case first */
	if (ap->ecount > 0) {
		ep = ap->elist[0];
		if (MEMCMP(e, ep->e, 6) == 0) {
			if (t - ep->t > NEWACTIVITY_DELTA) {
				report("new activity", a, e, NULL, &t, &ep->t,
				       interface);
				check_hname(ap);
			}
			ep->t = t;
			return (1);
		}
	}

	/* Check for a virgin ainfo record */
	if (ap->ecount == 0) {
		ap->ecount = 1;
		ap->elist[0] = elist_alloc(a, e, t, h, interface);
		report("new station", a, e, NULL, &t, NULL, interface);
		return (1);
	}

	/* Check for a flip-flop */
	if (ap->ecount > 1) {
		ep = ap->elist[1];
		if (MEMCMP(e, ep->e, 6) == 0) {
			/*
			 * Suppress report when less than
			 * FLIPFLOP_DELTA and one of the two ethernet
			 * addresses is a DECnet logical.
			 */
			t2 = ap->elist[0]->t;
			e2 = ap->elist[0]->e;
			if (t - t2 < FLIPFLIP_DELTA &&
			    (isdecnet(e) || isdecnet(e2)))
				dosyslog(LOG_INFO,
				    "suppressed DECnet flip flop", a, e, e2,
				    interface);
			else
				report("flip flop", a, e, e2, &t, &t2,
				       interface);
			ap->elist[1] = ap->elist[0];
			ap->elist[0] = ep;
			ep->t = t;
			check_hname(ap);
			return (1);
		}
	}

	for (i = 2; i < ap->ecount; ++i) {
		ep = ap->elist[i];
		if (MEMCMP(e, ep->e, 6) == 0) {
			/* An old entry comes to life */
			e2 = ap->elist[0]->e;
			t2 = ap->elist[0]->t;
			dosyslog(LOG_NOTICE, "reused old ethernet address",
			    a, e, e2, interface);
			/* Shift entries down */
			len = i * sizeof(ap->elist[0]);
			BCOPY(&ap->elist[0], &ap->elist[1], len);
			ap->elist[0] = ep;
			ep->t = t;
			check_hname(ap);
			return (1);
		}
	}

	/* New ether address */
	e2 = ap->elist[0]->e;
	t2 = ap->elist[0]->t;
	report("changed ethernet address", a, e, e2, &t, &t2, interface);
	/* Make room at head of list */
	alist_alloc(ap);
	len = ap->ecount * sizeof(ap->elist[0]);
	BCOPY(&ap->elist[0], &ap->elist[1], len);
	ap->elist[0] = elist_alloc(a, e, t, h, interface);
	++ap->ecount;
	return (1);
}