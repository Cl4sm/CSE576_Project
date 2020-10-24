static int
snprint_hw_failback (char * buff, int len, void * data)
{
	struct hwentry * hwe = (struct hwentry *)data;

	if (hwe->pgfailback == FAILBACK_UNDEF ||
	    hwe->pgfailback == DEFAULT_FAILBACK)
		return 0;

	switch(hwe->pgfailback) {
	case -FAILBACK_MANUAL:
		return snprintf(buff, len, "\"manual\"");
	case -FAILBACK_IMMEDIATE:
		return snprintf(buff, len, "\"immediate\"");
	case -FAILBACK_FOLLOWOVER:
		return snprintf(buff, len, "\"followover\"");
	default:
		return snprintf(buff, len, "%i", hwe->pgfailback);
	}
	return 0;
}
