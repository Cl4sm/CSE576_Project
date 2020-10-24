void
rxvt_set_jumpscroll( rxvt_t *r )
{
    if( r->h->rs[Rs_refreshLimit] )
    {
	r->h->refresh_limit = atol( r->h->rs[Rs_refreshLimit] );
	if( r->h->refresh_limit < 0 )
	    r->h->refresh_limit = 0;
    }
    else
	r->h->refresh_limit = DEFAULT_REFRESH_LIMIT;

    if( r->h->rs[Rs_skipPages] )
    {
	r->h->skip_pages = atol( r->h->rs[Rs_skipPages] );
	if( r->h->skip_pages <= 0 )
	    r->h->skip_pages = 1;
    }
    else
	r->h->skip_pages = DEFAULT_SKIP_PAGES;
}