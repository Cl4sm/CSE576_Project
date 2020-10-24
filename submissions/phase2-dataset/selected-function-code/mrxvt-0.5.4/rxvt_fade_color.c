void
rxvt_fade_color( rxvt_t* r, const XColor *xcol,
	unsigned long	*pix_return,
# ifdef XFT_SUPPORT
	XftColor	*xft_return
# else
	void		*xft_return
# endif
	)
{
    if( r->TermWin.fade )
    {
	XColor	faded_xcol;

	faded_xcol.red   = (xcol->red	/ 100) * r->TermWin.fade;
	faded_xcol.green = (xcol->green	/ 100) * r->TermWin.fade;
	faded_xcol.blue  = (xcol->blue	/ 100) * r->TermWin.fade;

	rxvt_alloc_color( r, &faded_xcol, "Faded" );

	*pix_return = faded_xcol.pixel;
# ifdef XFT_SUPPORT
	if( NOT_NULL( xft_return ) )
	    rxvt_alloc_xft_color( r, &faded_xcol, xft_return );

# endif
    }
}