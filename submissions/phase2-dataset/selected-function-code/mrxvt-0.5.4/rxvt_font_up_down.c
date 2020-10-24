void
rxvt_font_up_down(rxvt_t* r, int n, int direction)
{
    const char	 *p;
    int		    initial, j;

    for (j = 0; j < n; j++)
    {
	initial = r->h->fnum;
	for (;;)
	{
	    r->h->fnum += direction;
	    if (r->h->fnum == MAX_NFONTS || r->h->fnum == -1)
	    {
		r->h->fnum = initial;
		return;
	    }
	    p = r->h->rs[Rs_font + FNUM2IDX(r->h->fnum)];

	    if (NOT_NULL(p) && STRLEN(p) > 1)
		break;
	}
    }
}