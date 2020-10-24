static unsigned int get_proximity(struct sockaddr *host_addr)
{
	struct ifaddrs *ifa = NULL;
	struct ifaddrs *this;
	struct sockaddr_in *addr, *msk_addr, *if_addr;
	struct sockaddr_in6 *addr6, *msk6_addr, *if6_addr;
	struct in_addr *hst_addr;
	struct in6_addr *hst6_addr;
	int addr_len;
	char buf[MAX_ERR_BUF];
	uint32_t mask, ha, ia, *mask6, *ha6, *ia6;
	int ret;

	addr = NULL;
	addr6 = NULL;
	hst_addr = NULL;
	hst6_addr = NULL;
	mask6 = NULL;
	ha6 = NULL;
	ia6 = NULL;
	ha = 0;

	switch (host_addr->sa_family) {
	case AF_INET:
		addr = (struct sockaddr_in *) host_addr;
		hst_addr = (struct in_addr *) &addr->sin_addr;
		ha = ntohl((uint32_t) hst_addr->s_addr);
		addr_len = sizeof(*hst_addr);
		break;

	case AF_INET6:
#ifndef WITH_LIBTIRPC
		return PROXIMITY_UNSUPPORTED;
#else
		addr6 = (struct sockaddr_in6 *) host_addr;
		hst6_addr = (struct in6_addr *) &addr6->sin6_addr;
		ha6 = &hst6_addr->s6_addr32[0];
		addr_len = sizeof(*hst6_addr);
		break;
#endif

	default:
		return PROXIMITY_ERROR;
	}

	ret = getifaddrs(&ifa);
	if (ret) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr("getifaddrs: %s", estr);
		return PROXIMITY_ERROR;
	}

	this = ifa;
	while (this) {
		if (!(this->ifa_flags & IFF_UP) ||
		    this->ifa_flags & IFF_POINTOPOINT ||
		    this->ifa_addr == NULL) {
			this = this->ifa_next;
			continue;
		}

		switch (this->ifa_addr->sa_family) {
		case AF_INET:
			if (host_addr->sa_family == AF_INET6)
				break;
			if_addr = (struct sockaddr_in *) this->ifa_addr;
			ret = memcmp(&if_addr->sin_addr, hst_addr, addr_len);
			if (!ret) {
				freeifaddrs(ifa);
				return PROXIMITY_LOCAL;
			}
			break;

		case AF_INET6:
#ifdef WITH_LIBTIRPC
			if (host_addr->sa_family == AF_INET)
				break;
			if6_addr = (struct sockaddr_in6 *) this->ifa_addr;
			ret = memcmp(&if6_addr->sin6_addr, hst6_addr, addr_len);
			if (!ret) {
				freeifaddrs(ifa);
				return PROXIMITY_LOCAL;
			}
#endif
		default:
			break;
		}
		this = this->ifa_next;
	}

	this = ifa;
	while (this) {
		if (!(this->ifa_flags & IFF_UP) ||
		    this->ifa_flags & IFF_POINTOPOINT ||
		    this->ifa_addr == NULL) {
			this = this->ifa_next;
			continue;
		}

		switch (this->ifa_addr->sa_family) {
		case AF_INET:
			if (host_addr->sa_family == AF_INET6)
				break;
			if_addr = (struct sockaddr_in *) this->ifa_addr;
			ia =  ntohl((uint32_t) if_addr->sin_addr.s_addr);

			/* Is the address within a localy attached subnet */

			msk_addr = (struct sockaddr_in *) this->ifa_netmask;
			mask = ntohl((uint32_t) msk_addr->sin_addr.s_addr);

			if ((ia & mask) == (ha & mask)) {
				freeifaddrs(ifa);
				return PROXIMITY_SUBNET;
			}

			/*
			 * Is the address within a local ipv4 network.
			 *
			 * Bit position 31 == 0 => class A.
			 * Bit position 30 == 0 => class B.
			 * Bit position 29 == 0 => class C.
			 */

			if (!getbits(ia, 31, 1))
				mask = MASK_A;
			else if (!getbits(ia, 30, 1))
				mask = MASK_B;
			else if (!getbits(ia, 29, 1))
				mask = MASK_C;
			else
				break;

			if ((ia & mask) == (ha & mask)) {
				freeifaddrs(ifa);
				return PROXIMITY_NET;
			}
			break;

		case AF_INET6:
#ifdef WITH_LIBTIRPC
			if (host_addr->sa_family == AF_INET)
				break;
			if6_addr = (struct sockaddr_in6 *) this->ifa_addr;
			ia6 = &if6_addr->sin6_addr.s6_addr32[0];

			/* Is the address within the network of the interface */

			msk6_addr = (struct sockaddr_in6 *) this->ifa_netmask;
			mask6 = &msk6_addr->sin6_addr.s6_addr32[0];

			if (ipv6_mask_cmp(ha6, ia6, mask6)) {
				freeifaddrs(ifa);
				return PROXIMITY_SUBNET;
			}

			/* How do we define "local network" in ipv6? */
#endif
		default:
			break;
		}
		this = this->ifa_next;
	}

	freeifaddrs(ifa);

	return PROXIMITY_OTHER;
}