int openconnect_set_reported_os(struct openconnect_info *vpninfo, const char *os)
{
	if (!os) {
#if defined(__APPLE__)
		os = "mac-intel";
#elif defined(__ANDROID__)
		os = "android";
#else
		os = sizeof(long) > 4 ? "linux-64" : "linux";
#endif
	}

	if (!strcmp(os, "mac-intel"))
		vpninfo->csd_xmltag = "csdMac";
	else if (!strcmp(os, "linux") || !strcmp(os, "linux-64"))
		vpninfo->csd_xmltag = "csdLinux";
	else if (!strcmp(os, "android") || !strcmp(os, "apple-ios")) {
		vpninfo->csd_xmltag = "csdLinux";
		vpninfo->csd_nostub = 1;
	} else if (!strcmp(os, "win"))
		vpninfo->csd_xmltag = "csd";
	else
		return -EINVAL;

	vpninfo->platname = os;
	return 0;
}