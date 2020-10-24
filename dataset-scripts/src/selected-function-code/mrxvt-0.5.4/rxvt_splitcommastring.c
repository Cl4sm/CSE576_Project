char**
rxvt_splitcommastring(const char *cs)
{
    int             l, n, p;
    const char     *s, *t;
    char          **ret;

    if( IS_NULL(s = cs))
	s = "";

    for( n=1, t=s; *t; t++)
	if (*t == ',')
	    n++;

    assert (n >= 0 && n+1 > 0);	/* possible integer overflow? */

    ret = rxvt_malloc( (n + 1) * sizeof(char *) );
    ret[n] = NULL;

    for( l = 0, t = s; l < n; l++ )
    {
	for( ; *t && *t != ','; t++ );
	p = t - s;

	ret[l] = rxvt_malloc(p + 1);
	STRNCPY(ret[l], s, p);
	ret[l][p] = '\0';
	rxvt_str_trim(ret[l]);
	s = ++t;
    }
    return ret;
}