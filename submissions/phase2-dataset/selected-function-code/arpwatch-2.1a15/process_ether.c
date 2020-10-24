void
process_ether(register u_char *u, register const struct pcap_pkthdr *h,
    register const u_char *p)
{
	register struct ether_header *eh;
	register struct ether_arp *ea;
	register u_char *sea, *sha;
	register time_t t;
	u_int32_t sia;

	eh = (struct ether_header *)p;
	ea = (struct ether_arp *)(eh + 1);

	if (!sanity_ether(eh, ea, h->caplen))
		return;

	/* Source hardware ethernet address */
	sea = (u_char *)ESRC(eh);

	/* Source ethernet address */
	sha = (u_char *)SHA(ea);

	/* Source ip address */
	BCOPY(SPA(ea), &sia, 4);

	/* Watch for bogons */
	if (isbogon(sia)) {
		dosyslog(LOG_INFO, "bogon", sia, sea, sha, interface);
		if (!allsubnets) return;
	}

	/* Watch for ethernet broadcast */
	if (MEMCMP(sea, zero, 6) == 0 || MEMCMP(sea, allones, 6) == 0 ||
	    MEMCMP(sha, zero, 6) == 0 || MEMCMP(sha, allones, 6) == 0) {
		dosyslog(LOG_INFO, "ethernet broadcast", sia, sea, sha,
			 interface);
		return;
	}

	/* Double check ethernet addresses */
	if (MEMCMP(sea, sha, 6) != 0) {
		dosyslog(LOG_INFO, "ethernet mismatch", sia, sea, sha,
			 interface);
		return;
	}

	/* Ignores the specified netmask/metwork */
	if ((sia & ignore_netmask.s_addr) == ignore_net.s_addr) {
		if (debug) {
			dosyslog(LOG_INFO, "ignored", sia, sea, sha, interface);
		}
		return;
	}
	/* Got a live one */

	t = h->ts.tv_sec;
	can_checkpoint = 0;
	if (!ent_add(sia, sea, t, NULL, interface))
		syslog(LOG_ERR, "ent_add(%s, %s, %ld, %s) failed",
		    intoa(sia), e2str(sea), t, interface);
	can_checkpoint = 1;
}