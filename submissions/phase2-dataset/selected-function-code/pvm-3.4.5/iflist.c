int
iflist(alp, np)
	struct in_addr **alp;	/* return list of addresses */
	int *np;				/* return len of alp */
{
	int soc = -1;						/* socket */
	static struct in_addr *iplist = 0;	/* list of interface addrs found */
	int nip = 0;						/* length of iplist */
	char buf[4096];						/* return space for SIOCGIOCONF */
	struct ifconf sif;
	struct ifreq *reqp;
	struct ifreq req;
	char *cp;

	if (iplist)
		PVM_FREE(iplist);
	iplist = TALLOC(10, struct in_addr, "ifl");

	if ((soc = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		goto bail;
	}

	sif.ifc_ifcu.ifcu_buf = buf;
	sif.ifc_len = sizeof(buf);
	if (ioctl(soc,
#ifdef OSIOCGIFCONF
#if defined(IMA_RS6K) || defined(IMA_RS6KMP) || defined(IMA_SP2MPI) \
			|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
			|| defined(IMA_FREEBSD)
		SIOCGIFCONF
#else
		OSIOCGIFCONF
#endif
#else
		SIOCGIFCONF
#endif
		, &sif) == -1) {
		perror("ioctl");
		goto bail;
	}

	for (cp = sif.ifc_ifcu.ifcu_buf;
			cp - sif.ifc_ifcu.ifcu_buf < sif.ifc_len;
			cp += sizeof(*reqp) - sizeof(struct sockaddr) + SIZ(reqp->ifr_addr))
	{
		reqp = (struct ifreq*)cp;
		if (reqp->ifr_addr.sa_family != AF_INET)
			continue;
		BCOPY(reqp->ifr_name, req.ifr_name, sizeof(req.ifr_name));
		if (ioctl(soc, SIOCGIFFLAGS, &req) == -1) {
			perror("ioctl");
			goto bail;
		}
		/* On some FreeBSD systems: */
		/* if (IFF_UP & req.ifr_flags) { */
		if (IFF_UP & req.ifr_ifru.ifru_flags) {
			if (nip > 0 && !(nip % 10))
				iplist = TREALLOC(iplist, (nip + 10), struct in_addr);
			iplist[nip++] =
			((struct sockaddr_in*)(&reqp->ifr_ifru.ifru_addr))->sin_addr;
			if (pvmdebmask & PDMNET) {
				long a;

				a = ((struct sockaddr_in*)(&reqp->ifr_ifru.ifru_addr))->sin_addr.s_addr;
				a = ntohl(a);
				pvmlogprintf("iflist() %s %d.%d.%d.%d\n",
						reqp->ifr_name,
						0xff & (a >> 24),
						0xff & (a >> 16),
						0xff & (a >> 8),
						0xff & a);
			}
		}
	}

	*alp = iplist;
	*np = nip;
	return 0;

bail:
	(void)close(soc);
	return -1;
}
