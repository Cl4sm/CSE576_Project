int
sanity_fddi(register struct fddi_header *fh, register struct ether_arp *ea,
    register int len)
{
	u_char *shost = fh->src;
	u_short type, hrd, pro, op;

	/* This rather clunky copy stuff is needed because the fddi header
	 * has an odd (i.e. not even) length, causing memory alignment
	 * errors when attempts are made to access the arp header fields
	 * as shorts */
	BCOPY(fh->snap.snap_type, &type, sizeof(u_short));
	BCOPY(&(ea->arp_hrd), &hrd, sizeof(hrd));
	BCOPY(&(ea->arp_pro), &pro, sizeof(pro));
	BCOPY(&(ea->arp_op), &op, sizeof(op));
	type = ntohs(type);
	hrd = ntohs(hrd);
	pro = ntohs(pro);
	op = ntohs(op);

	if (len < sizeof(*fh) + sizeof(*ea)) {
		syslog(LOG_ERR, "short (want %d)\n", sizeof(*fh) + sizeof(*ea));
		return(0);
	}

	/* XXX sysv r4 seems to use hardware format 6 */
	if (hrd != ARPHRD_ETHER && hrd != 6) {
		syslog(LOG_ERR, "%s sent bad hardware format 0x%x\n",
		    e2str(shost), hrd);
		return(0);
	}


	/* XXX hds X terminals sometimes send trailer arp replies */
	if (pro != ETHERTYPE_IP &&
	    pro != ETHERTYPE_TRAIL &&
	    pro != ETHERTYPE_APOLLO) {
		syslog(LOG_ERR, "%s sent packet not ETHERTYPE_IP (0x%x)\n",
		    e2str(shost), pro);
		return(0);
	}

	if (ea->arp_hln != 6 || ea->arp_pln != 4) {
		syslog(LOG_ERR, "%s sent bad addr len (hard %d, prot %d)\n",
		    e2str(shost), ea->arp_hln, ea->arp_pln);
		return(0);
	}

	/*
	 * We're only interested in arp requests, arp replies
	 * and reverse arp replies
	 */
	if (type == ETHERTYPE_ARP) {
		if (op != ARPOP_REQUEST &&
		    op != ARPOP_REPLY) {
			syslog(LOG_ERR, "%s sent wrong arp op %d\n",
			     e2str(shost), op);
			return(0);
		}
	} else if (type == ETHERTYPE_REVARP) {
		if (op == REVARP_REQUEST) {
			/* no useful information here */
			return(0);
		} else if (op != REVARP_REPLY) {
			if (debug)
				syslog(LOG_ERR, "%s sent wrong revarp op %d\n",
				    e2str(shost), op);
			return(0);
		}
	} else {
		syslog(LOG_ERR, "%s sent bad type 0x%x\n",
		    e2str(shost), type);
		return(0);
	}
	return(1);
}