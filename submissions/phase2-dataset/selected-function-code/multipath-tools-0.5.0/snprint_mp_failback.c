snprint_mp_failback (char * buff, int len, void * data)
{
	struct mpentry * mpe = (struct mpentry *)data;

	if (mpe->pgfailback == FAILBACK_UNDEF ||
	    mpe->pgfailback == DEFAULT_FAILBACK)
		return 0;

	switch(mpe->pgfailback) {
	case -FAILBACK_MANUAL:
		return snprintf(buff, len, "\"manual\"");
	case -FAILBACK_IMMEDIATE:
		return snprintf(buff, len, "\"immediate\"");
	case -FAILBACK_FOLLOWOVER:
		return snprintf(buff, len, "\"followover\"");
	default:
		return snprintf(buff, len, "%i", mpe->pgfailback);
	}
	return 0;
}
