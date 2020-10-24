extern int
snprint_blacklist_report (char * buff, int len)
{
	int threshold = MAX_LINE_LEN;
	int fwd = 0;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "device node rules:\n"
					       "- blacklist:\n");
	if (!snprint_blacklist_group(buff, len, &fwd, &conf->blist_devnode))
		return len;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "- exceptions:\n");
	if (snprint_blacklist_group(buff, len, &fwd, &conf->elist_devnode) == 0)
		return len;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "udev property rules:\n"
					       "- blacklist:\n");
	if (!snprint_blacklist_group(buff, len, &fwd, &conf->blist_property))
		return len;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "- exceptions:\n");
	if (snprint_blacklist_group(buff, len, &fwd, &conf->elist_property) == 0)
		return len;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "wwid rules:\n"
					       "- blacklist:\n");
	if (snprint_blacklist_group(buff, len, &fwd, &conf->blist_wwid) == 0)
		return len;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "- exceptions:\n");
	if (snprint_blacklist_group(buff, len, &fwd, &conf->elist_wwid) == 0)
		return len;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "device rules:\n"
					       "- blacklist:\n");
	if (snprint_blacklist_devgroup(buff, len, &fwd, &conf->blist_device) == 0)
		return len;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "- exceptions:\n");
	if (snprint_blacklist_devgroup(buff, len, &fwd, &conf->elist_device) == 0)
		return len;

	if (fwd > len)
		return len;
	return fwd;
}
