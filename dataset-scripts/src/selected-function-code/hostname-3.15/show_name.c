void
show_name(enum type_t type)
{
	struct addrinfo *res;
	struct addrinfo hints;
	struct ifaddrs *ifa, *ifap;
	char *p;
	int ret;

	/* Handle a few cases specially. */
	switch(type)
	{
		case DEFAULT: 
			printf("%s\n", localhost());
			break;
		case SHORT:
			p = localhost();
			*(strchrnul(p, '.')) = '\0';
			printf("%s\n", p);
			break;
		case NIS:
			printf("%s\n", localdomain());
			break;
		case NIS_DEF:
			printf("%s\n", localnisdomain());
			break;
		case ALL_IPS:
		case ALL_FQDNS: {
			char buf[INET6_ADDRSTRLEN];
			int flags, ret, family, addrlen;

			/* What kind of information do we want from getnameinfo()? */
			flags = (type == ALL_IPS) ? NI_NUMERICHOST : NI_NAMEREQD;

			if (getifaddrs(&ifa) != 0)
				errx(1, "%s", strerror(errno));
			for (ifap = ifa; ifap != NULL; ifap = ifap->ifa_next) {
				/* Skip interfaces that have no configured addresses */
				if (ifap->ifa_addr == NULL)
					continue;
				/* Skip the loopback interface */
				if (ifap->ifa_flags & IFF_LOOPBACK)
					continue;
				/* Skip interfaces that are not UP */
				if (!(ifap->ifa_flags & IFF_UP))
					continue;

				/* Only handle IPv4 and IPv6 addresses */
				family = ifap->ifa_addr->sa_family;
				if (family != AF_INET && family != AF_INET6)
					continue;

				addrlen = (family == AF_INET) ? sizeof(struct sockaddr_in) :
								sizeof(struct sockaddr_in6);

				/* Skip IPv6 link-local addresses */
				if (family == AF_INET6) {
					struct sockaddr_in6 *sin6;

					sin6 = (struct sockaddr_in6 *)ifap->ifa_addr;
					if (IN6_IS_ADDR_LINKLOCAL(&sin6->sin6_addr) ||
							IN6_IS_ADDR_MC_LINKLOCAL(&sin6->sin6_addr))
						continue;
				}

				ret = getnameinfo(ifap->ifa_addr, addrlen,
						  buf, sizeof(buf), NULL, 0, flags);

				/* Just skip addresses that cannot be translated */
				if (ret != 0) {
				    if (type != ALL_FQDNS && ret != EAI_NONAME)
					errx(1, "%s", gai_strerror(ret));
				} else
					printf("%s ", buf);
			}
			printf("\n");
			freeifaddrs(ifa);
			break;
		}
		default:
			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_CANONNAME;

			p = localhost();
			if ((ret = getaddrinfo(p, NULL, &hints, &res)) != 0)
				errx(1, "%s", gai_strerror(ret));
			
			p = strchr(res->ai_canonname, '.');

			switch (type) {
			case ALIAS: {
				struct hostent *hp;
				int i;

				if ((hp = gethostbyname(localhost())) == NULL)
					errx(1, "%s", hstrerror(h_errno));

				for (i = 0; hp->h_aliases[i]; i++) {
					if (i > 0)
						printf(" ");
					printf("%s", hp->h_aliases[i]);
				}
				printf("\n");
				break;
			}

			case IP: {
				char buf[INET6_ADDRSTRLEN];
				int ret;

				struct addrinfo *walk;

				for (walk = res; walk != NULL; walk = walk->ai_next) {
					if ((ret = getnameinfo(walk->ai_addr, walk->ai_addrlen,
								buf, sizeof(buf), NULL, 0,
								NI_NUMERICHOST)) != 0)
						errx(1, "%s", gai_strerror(ret));

					if (walk != res)
						printf(" ");

					printf("%s", buf);
				}
				printf("\n");
				break;
			}

			case DNS:
				if (p != NULL)
					printf("%s\n", ++p);
				break;

			case FQDN:
				printf("%s\n", res->ai_canonname);
				break;

			default:
				break;
			}
			break;
	}
}