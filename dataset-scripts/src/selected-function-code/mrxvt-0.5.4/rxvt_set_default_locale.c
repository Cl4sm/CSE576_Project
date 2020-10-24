void
rxvt_set_default_locale (rxvt_t* r)
{
    char*   locale;
#if defined(HAVE_SETLOCALE) || defined(HAVE_XSETLOCALE)
    char*   lc;
#endif

    locale = getenv ("LC_ALL");
    if (IS_NULL(locale))
	locale = getenv ("LC_CTYPE");
    if (IS_NULL(locale))
	locale = getenv ("LANG");
    
#if defined(HAVE_SETLOCALE) || defined(HAVE_XSETLOCALE)
    lc = setlocale(LC_CTYPE, "");
    if (IS_NULL(locale))
	locale = lc;
#endif

    rxvt_msg (DBG_INFO, DBG_ENCODING, "set default locale to %s\n", locale ? locale : "none");
    r->h->locale = locale;
}