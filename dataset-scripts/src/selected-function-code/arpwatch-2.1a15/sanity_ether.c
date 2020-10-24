int
sanity_ether(register struct ether_header *eh, register struct ether_arp *ea,
    register int len)
{
	/* XXX use bsd style ether_header to avoid messy ifdef's */
	struct bsd_ether_header {
		u_char  ether_dhost[6];
		u_char  ether_shost[6];
		u_short ether_type;
	};
	register u_char *shost = ((struct bsd_ether_header *)eh)->ether_shost;

	eh->ether_type = ntohs(eh->ether_type);
	ea->arp_hrd = ntohs(ea->arp_hrd);
	ea->arp_pro = ntohs(ea->arp_pro);
	ea->arp_op = ntohs(ea->arp_op);

	if (len < sizeof(*eh) + sizeof(*ea)) {
		syslog(LOG_ERR, "short (want %d)\n", sizeof(*eh) + sizeof(*ea));
		return(0);
	}

	/* XXX sysv r4 seems to use hardware format 6 */
	if (ea->arp_hrd != ARPHRD_ETHER && ea->arp_hrd != 6) {
		syslog(LOG_ERR, "%s sent bad hardware format 0x%x\n",
		    e2str(shost), ea->arp_hrd);
		return(0);
	}

	/* XXX hds X terminals sometimes send trailer arp replies */
	if (ea->arp_pro != ETHERTYPE_IP && ea->arp_pro != ETHERTYPE_TRAIL) {
		syslog(LOG_ERR, "%s sent packet not ETHERTYPE_IP (0x%x)\n",
		    e2str(shost), ea->arp_pro);
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
	if (eh->ether_type == ETHERTYPE_ARP) {
		if (ea->arp_op != ARPOP_REQUEST &&
		    ea->arp_op != ARPOP_REPLY) {
			syslog(LOG_ERR, "%s sent wrong arp op %d\n",
			     e2str(shost), ea->arp_op);
			return(0);
		}
	} else if (eh->ether_type == ETHERTYPE_REVARP) {
		if (ea->arp_op == REVARP_REQUEST) {
			/* no useful information here */
			return(0);
		} else if (ea->arp_op != REVARP_REPLY) {
			if (debug)
				syslog(LOG_ERR, "%s sent wrong revarp op %d\n",
				    e2str(shost), ea->arp_op);
			return(0);
		}
	} else {
		syslog(LOG_ERR, "%s sent bad type 0x%x\n",
		    e2str(shost), eh->ether_type);
		return(0);
	}

	return(1);
}