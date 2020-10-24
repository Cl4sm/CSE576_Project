static void calculate_mtu(struct openconnect_info *vpninfo, int *base_mtu, int *mtu)
{
	*mtu = vpninfo->reqmtu;
	*base_mtu = vpninfo->basemtu;

#if defined(__linux__) && defined(TCP_INFO)
	if (!*mtu || !*base_mtu) {
		struct tcp_info ti;
		socklen_t ti_size = sizeof(ti);

		if (!getsockopt(vpninfo->ssl_fd, IPPROTO_TCP, TCP_INFO,
				&ti, &ti_size)) {
			vpn_progress(vpninfo, PRG_DEBUG,
				     _("TCP_INFO rcv mss %d, snd mss %d, adv mss %d, pmtu %d\n"),
				     ti.tcpi_rcv_mss, ti.tcpi_snd_mss, ti.tcpi_advmss, ti.tcpi_pmtu);
			if (!*base_mtu)
				*base_mtu = ti.tcpi_pmtu;
			if (!*mtu) {
				if (ti.tcpi_rcv_mss < ti.tcpi_snd_mss)
					*mtu = ti.tcpi_rcv_mss - 13;
				else
					*mtu = ti.tcpi_snd_mss - 13;
			}
		}
	}
#endif
#ifdef TCP_MAXSEG
	if (!*mtu) {
		int mss;
		socklen_t mss_size = sizeof(mss);
		if (!getsockopt(vpninfo->ssl_fd, IPPROTO_TCP, TCP_MAXSEG,
				&mss, &mss_size)) {
			vpn_progress(vpninfo, PRG_DEBUG, _("TCP_MAXSEG %d\n"), mss);
			*mtu = mss - 13;
		}
	}
#endif
	if (!*mtu) {
		/* Default */
		*mtu = 1406;
	}
	if (*mtu < 1280)
		*mtu = 1280;
}