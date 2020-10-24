static intptr_t open_tun(struct openconnect_info *vpninfo, char *guid, char *name)
{
	char devname[80];
	HANDLE tun_fh;
	ULONG data[3];
	DWORD len;

	snprintf(devname, sizeof(devname), DEVTEMPLATE, guid);
	tun_fh = CreateFile(devname, GENERIC_WRITE|GENERIC_READ, 0, 0,
			    OPEN_EXISTING,
			    FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED,
			    0);
	if (tun_fh == INVALID_HANDLE_VALUE) {
		vpn_progress(vpninfo, PRG_ERR, _("Failed to open %s\n"),
			     devname);
		return -1;

	}
	vpn_progress(vpninfo, PRG_DEBUG, _("Opened tun device %s\n"), devname);

	if (!DeviceIoControl(tun_fh, TAP_IOCTL_GET_VERSION,
			     NULL, 0, data, sizeof(data), &len, NULL)) {
		DWORD err = GetLastError();

		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to obtain TAP driver version: %lx\n"), err);
		return -1;
	}
	if (data[0] < 9 || (data[0] == 9 && data[1] < 9)) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error: TAP-Windows driver v9.9 or greater is required (found %ld.%ld)\n"),
			     data[0], data[1]);
		return -1;
	}
	vpn_progress(vpninfo, PRG_DEBUG, "TAP-Windows driver v%ld.%ld (%ld)\n",
		     data[0], data[1], data[2]);

	data[0] = inet_addr(vpninfo->ip_info.addr);
	data[2] = inet_addr(vpninfo->ip_info.netmask);
	data[1] = data[0] & data[2];

	if (!DeviceIoControl(tun_fh, TAP_IOCTL_CONFIG_TUN,
			     data, sizeof(data), NULL, 0, &len, NULL)) {
		DWORD err = GetLastError();

		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to set TAP IP addresses: %lx\n"), err);
		return -1;
	}

	data[0] = 1;
	if (!DeviceIoControl(tun_fh, TAP_IOCTL_SET_MEDIA_STATUS,
			     data, sizeof(data[0]), NULL, 0, &len, NULL)) {
		DWORD err = GetLastError();

		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to set TAP media status: %lx\n"), err);
		return -1;
	}
	if (!vpninfo->ifname)
		vpninfo->ifname = strdup(name);

	return (intptr_t)tun_fh;
}