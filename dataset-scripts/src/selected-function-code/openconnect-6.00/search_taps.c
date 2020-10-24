static intptr_t search_taps(struct openconnect_info *vpninfo, tap_callback *cb, int all)
{
	LONG status;
	HKEY adapters_key;
	DWORD len;
	char buf[40], name[40];
	char keyname[strlen(CONNECTIONS_KEY) + sizeof(buf) + 1 + strlen("\\Connection")];
	int i = 0, found = 0;
	intptr_t ret;

	status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, ADAPTERS_KEY, 0,
			      KEY_READ, &adapters_key);
	if (status) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error accessing registry key for network adapters\n"));
		return -EIO;
	}
	while (1) {
		len = sizeof(buf);
		status = RegEnumKeyEx(adapters_key, i++, buf, &len,
				       NULL, NULL, NULL, NULL);
		if (status) {
			if (status != ERROR_NO_MORE_ITEMS)
				ret = -1;
			break;
		}

		snprintf(keyname, sizeof(keyname), "%s\\%s",
			 ADAPTERS_KEY, buf);

		len = sizeof(buf);
		status = RegGetValue(HKEY_LOCAL_MACHINE, keyname,
				     "ComponentId", RRF_RT_REG_SZ,
				     NULL, buf, &len);
		if (status || strcmp(buf, TAP_COMPONENT_ID))
			continue;

		len = sizeof(buf);
		status = RegGetValue(HKEY_LOCAL_MACHINE, keyname,
				     "NetCfgInstanceId", RRF_RT_REG_SZ,
				     NULL, buf, &len);
		if (status)
			continue;

		snprintf(keyname, sizeof(keyname), "%s\\%s\\Connection",
			 CONNECTIONS_KEY, buf);

		len = sizeof(name);
		status = RegGetValue(HKEY_LOCAL_MACHINE, keyname, "Name",
				     RRF_RT_REG_SZ, NULL, name, &len);
		if (status)
			continue;

		found++;

		if (vpninfo->ifname && strcmp(name, vpninfo->ifname)) {
			vpn_progress(vpninfo, PRG_DEBUG,
				     _("Ignoring non-matching TAP interface \"%s\""),
				     name);
			continue;
		}

		ret = cb(vpninfo, buf, name);
		if (!all)
			break;
	}

	RegCloseKey(adapters_key);

	if (!found)
		vpn_progress(vpninfo, PRG_ERR,
			     _("No Windows-TAP adapters found. Is the driver installed?\n"));

	return ret;
}