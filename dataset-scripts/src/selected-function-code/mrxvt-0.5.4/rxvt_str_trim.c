char*
rxvt_str_trim(char *str)
{
    char           *r, *s;
    int             n;

    if (!str || !*str)	    /* shortcut */
	return str;

    /* skip leading spaces */
    for (s = str; *s && isspace((int) *s); s++)
	;
    /* goto end of string */
    for (n = 0, r = s; *r++; n++)
	;
    r -= 2;
    /* dump return */
    if (n > 0 && *r == '\n')
	n--, r--;
    /* backtrack along trailing spaces */
    for (; n > 0 && isspace((int) *r); r--, n--)
	;
    /* skip matching leading/trailing quotes */
    if( *s == '"' && *r == '"' && n > 1 )
    {
	s++;
	n -= 2;
    }
    /* copy back over: forwards copy */
    for (r = str; n; n--)
	*r++ = *s++;
    *r = '\0';

    return str;
}