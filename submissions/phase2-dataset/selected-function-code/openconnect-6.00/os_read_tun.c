int os_read_tun(struct openconnect_info *vpninfo, struct pkt *pkt)
{
	DWORD pkt_size;

 reread:
	if (!vpninfo->tun_rd_pending &&
	    !ReadFile(vpninfo->tun_fh, pkt->data, pkt->len, &pkt_size,
		      &vpninfo->tun_rd_overlap)) {
		DWORD err = GetLastError();

		if (err == ERROR_IO_PENDING)
			vpninfo->tun_rd_pending = 1;
		else
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to read from TAP device: %lx\n"),
				     err);
		return -1;
	} else if (!GetOverlappedResult(vpninfo->tun_fh,
					&vpninfo->tun_rd_overlap, &pkt_size,
					FALSE)) {
		DWORD err = GetLastError();

		if (err != ERROR_IO_INCOMPLETE) {
			vpninfo->tun_rd_pending = 0;
			vpn_progress(vpninfo, PRG_ERR,
				     _("Failed to complete read from TAP device: %lx\n"),
				     err);
			goto reread;
		}
		return -1;
	}

	/* Either a straight ReadFile() or a subsequent GetOverlappedResult()
	   succeeded... */
	vpninfo->tun_rd_pending = 0;
	pkt->len = pkt_size;
	return 0;
}