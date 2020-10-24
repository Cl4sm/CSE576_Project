static void add_common_headers(struct openconnect_info *vpninfo, struct oc_text_buf *buf)
{
	struct oc_vpn_option *opt;

	buf_append(buf, "Host: %s\r\n", vpninfo->hostname);
	buf_append(buf, "User-Agent: %s\r\n", vpninfo->useragent);
	buf_append(buf, "Accept: */*\r\n");
	buf_append(buf, "Accept-Encoding: identity\r\n");

	if (vpninfo->cookies) {
		buf_append(buf, "Cookie: ");
		for (opt = vpninfo->cookies; opt; opt = opt->next)
			buf_append(buf, "%s=%s%s", opt->option,
				      opt->value, opt->next ? "; " : "\r\n");
	}
	buf_append(buf, "X-Transcend-Version: 1\r\n");
	if (vpninfo->xmlpost) {
		buf_append(buf, "X-Aggregate-Auth: 1\r\n");
		buf_append(buf, "X-AnyConnect-Platform: %s\r\n",
			   vpninfo->platname);
	}
	if (vpninfo->mobile_platform_version) {
		buf_append(buf, "X-AnyConnect-Identifier-ClientVersion: %s\r\n",
			   openconnect_version_str);
		buf_append(buf, "X-AnyConnect-Identifier-Platform: %s\r\n",
			   vpninfo->platname);
		buf_append(buf, "X-AnyConnect-Identifier-PlatformVersion: %s\r\n",
			   vpninfo->mobile_platform_version);
		buf_append(buf, "X-AnyConnect-Identifier-DeviceType: %s\r\n",
			   vpninfo->mobile_device_type);
		buf_append(buf, "X-AnyConnect-Identifier-Device-UniqueID: %s\r\n",
			   vpninfo->mobile_device_uniqueid);
	}
}