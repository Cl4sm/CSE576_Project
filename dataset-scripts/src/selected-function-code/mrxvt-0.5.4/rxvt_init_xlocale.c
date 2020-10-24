void
rxvt_init_xlocale(rxvt_t *r)
{
#ifdef USE_XIM
    if (IS_NULL(r->h->locale))
	rxvt_msg (DBG_ERROR, DBG_INIT, "Setting locale failed.");
    else
    {
	XChangeProperty(r->Xdisplay, r->TermWin.parent,
	    r->h->xa[XA_WM_LOCALE_NAME], XA_STRING, 8, PropModeReplace,
	    (unsigned char *)r->h->locale, STRLEN(r->h->locale));

	if (XSupportsLocale() != True)
	{
	    rxvt_msg (DBG_ERROR, DBG_INIT, "The locale is not supported by Xlib");
	    return;
	}
	rxvt_IM_set_fontset (r, 0);

	/* see if we can connect yet */
	rxvt_IM_init_callback (r->Xdisplay, NULL, NULL);

	/* To avoid Segmentation Fault in C locale: Solaris only? */
	if (STRCMP(r->h->locale, "C"))
	    XRegisterIMInstantiateCallback(r->Xdisplay, NULL, NULL,
		NULL, rxvt_IM_init_callback, NULL);
    }
#endif
}