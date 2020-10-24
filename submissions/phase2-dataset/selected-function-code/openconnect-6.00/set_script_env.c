void set_script_env(struct openconnect_info *vpninfo)
{
	char host[80];
	int ret = getnameinfo(vpninfo->peer_addr, vpninfo->peer_addrlen, host,
			      sizeof(host), NULL, 0, NI_NUMERICHOST);
	if (!ret)
		setenv("VPNGATEWAY", host, 1);

	set_banner(vpninfo);
	unsetenv("CISCO_SPLIT_INC");
	unsetenv("CISCO_SPLIT_EXC");

	setenv_int("INTERNAL_IP4_MTU", vpninfo->ip_info.mtu);

	if (vpninfo->ip_info.addr) {
		setenv("INTERNAL_IP4_ADDRESS", vpninfo->ip_info.addr, 1);
		if (vpninfo->ip_info.netmask) {
			struct in_addr addr;
			struct in_addr mask;

			if (inet_aton(vpninfo->ip_info.addr, &addr) &&
			    inet_aton(vpninfo->ip_info.netmask, &mask)) {
				char *netaddr;

				addr.s_addr &= mask.s_addr;
				netaddr = inet_ntoa(addr);

				setenv("INTERNAL_IP4_NETADDR", netaddr, 1);
				setenv("INTERNAL_IP4_NETMASK", vpninfo->ip_info.netmask, 1);
				setenv_int("INTERNAL_IP4_NETMASKLEN", netmasklen(mask));
			}
		}
	}
	if (vpninfo->ip_info.addr6) {
		setenv("INTERNAL_IP6_ADDRESS", vpninfo->ip_info.addr6, 1);
		setenv("INTERNAL_IP6_NETMASK", vpninfo->ip_info.netmask6, 1);
	} else if (vpninfo->ip_info.netmask6) {
               char *slash = strchr(vpninfo->ip_info.netmask6, '/');
               setenv("INTERNAL_IP6_NETMASK", vpninfo->ip_info.netmask6, 1);
               if (slash) {
                       *slash = 0;
                       setenv("INTERNAL_IP6_ADDRESS", vpninfo->ip_info.netmask6, 1);
                       *slash = '/';
               }
	}

	if (vpninfo->ip_info.dns[0])
		setenv("INTERNAL_IP4_DNS", vpninfo->ip_info.dns[0], 1);
	else
		unsetenv("INTERNAL_IP4_DNS");
	if (vpninfo->ip_info.dns[1])
		appendenv("INTERNAL_IP4_DNS", vpninfo->ip_info.dns[1]);
	if (vpninfo->ip_info.dns[2])
		appendenv("INTERNAL_IP4_DNS", vpninfo->ip_info.dns[2]);

	if (vpninfo->ip_info.nbns[0])
		setenv("INTERNAL_IP4_NBNS", vpninfo->ip_info.nbns[0], 1);
	else
		unsetenv("INTERNAL_IP4_NBNS");
	if (vpninfo->ip_info.nbns[1])
		appendenv("INTERNAL_IP4_NBNS", vpninfo->ip_info.nbns[1]);
	if (vpninfo->ip_info.nbns[2])
		appendenv("INTERNAL_IP4_NBNS", vpninfo->ip_info.nbns[2]);

	if (vpninfo->ip_info.domain)
		setenv("CISCO_DEF_DOMAIN", vpninfo->ip_info.domain, 1);
	else
		unsetenv("CISCO_DEF_DOMAIN");

	if (vpninfo->ip_info.proxy_pac)
		setenv("CISCO_PROXY_PAC", vpninfo->ip_info.proxy_pac, 1);

	if (vpninfo->ip_info.split_dns) {
		char *list;
		int len = 0;
		struct oc_split_include *dns = vpninfo->ip_info.split_dns;

		while (dns) {
			len += strlen(dns->route) + 1;
			dns = dns->next;
		}
		list = malloc(len);
		if (list) {
			char *p = list;

			dns = vpninfo->ip_info.split_dns;
			while (1) {
				strcpy(p, dns->route);
				p += strlen(p);
				dns = dns->next;
				if (!dns)
					break;
				*(p++) = ',';
			}
			setenv("CISCO_SPLIT_DNS", list, 1);
			free(list);
		}
	}
	if (vpninfo->ip_info.split_includes) {
		struct oc_split_include *this = vpninfo->ip_info.split_includes;
		int nr_split_includes = 0;
		int nr_v6_split_includes = 0;

		while (this) {
			process_split_xxclude(vpninfo, 1, this->route,
					      &nr_split_includes,
					      &nr_v6_split_includes);
			this = this->next;
		}
		if (nr_split_includes)
			setenv_int("CISCO_SPLIT_INC", nr_split_includes);
		if (nr_v6_split_includes)
			setenv_int("CISCO_IPV6_SPLIT_INC", nr_v6_split_includes);
	}
	if (vpninfo->ip_info.split_excludes) {
		struct oc_split_include *this = vpninfo->ip_info.split_excludes;
		int nr_split_excludes = 0;
		int nr_v6_split_excludes = 0;

		while (this) {
			process_split_xxclude(vpninfo, 0, this->route,
					      &nr_split_excludes,
					      &nr_v6_split_excludes);
			this = this->next;
		}
		if (nr_split_excludes)
			setenv_int("CISCO_SPLIT_EXC", nr_split_excludes);
		if (nr_v6_split_excludes)
			setenv_int("CISCO_IPV6_SPLIT_EXC", nr_v6_split_excludes);
	}
	setenv_cstp_opts(vpninfo);
}