void cstp_free_splits(struct openconnect_info *vpninfo)
{
	struct oc_split_include *inc;

	for (inc = vpninfo->ip_info.split_includes; inc; ) {
		struct oc_split_include *next = inc->next;
		free(inc);
		inc = next;
	}
	for (inc = vpninfo->ip_info.split_excludes; inc; ) {
		struct oc_split_include *next = inc->next;
		free(inc);
		inc = next;
	}
	for (inc = vpninfo->ip_info.split_dns; inc; ) {
		struct oc_split_include *next = inc->next;
		free(inc);
		inc = next;
	}
	vpninfo->ip_info.split_dns = vpninfo->ip_info.split_includes =
		vpninfo->ip_info.split_excludes = NULL;
}