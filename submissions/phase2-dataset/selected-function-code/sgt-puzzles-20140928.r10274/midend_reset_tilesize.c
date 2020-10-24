void midend_reset_tilesize(midend *me)
{
    me->preferred_tilesize = me->ourgame->preferred_tilesize;
    {
        /*
         * Allow an environment-based override for the default tile
         * size by defining a variable along the lines of
         * `NET_TILESIZE=15'.
         */

	char buf[80], *e;
	int j, k, ts;

	sprintf(buf, "%s_TILESIZE", me->ourgame->name);
	for (j = k = 0; buf[j]; j++)
	    if (!isspace((unsigned char)buf[j]))
		buf[k++] = toupper((unsigned char)buf[j]);
	buf[k] = '\0';
	if ((e = getenv(buf)) != NULL && sscanf(e, "%d", &ts) == 1 && ts > 0)
	    me->preferred_tilesize = ts;
    }
}