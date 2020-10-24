void
rxvt_free_hidden( rxvt_t* r )
{
#ifdef DEBUG
    if (IS_CURSOR(r->h->bar_pointer))
    {
	XFreeCursor( r->Xdisplay, r->h->bar_pointer );
	UNSET_CURSOR(r->h->bar_pointer);
    }
# ifdef POINTER_BLANK
    if (IS_CURSOR(r->h->blank_pointer))
    {
	XFreeCursor( r->Xdisplay, r->h->blank_pointer );
	UNSET_CURSOR(r->h->blank_pointer);
    }
# endif
#endif	/* DEBUG */

#ifdef USE_XIM
    if( r->h->Input_Context )   
    {
	XDestroyIC( r->h->Input_Context );
	SET_NULL(r->h->Input_Context); 
    }
#endif
}