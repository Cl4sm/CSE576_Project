char* 
rxvt_get_encoding_from_locale (rxvt_t* r)
{
    char*   enc;

    if (IS_NULL(r->h->locale))
	return NULL;

#ifdef HAVE_NL_LANGINFO
    if (IS_NULL (enc = nl_langinfo (CODESET)))
	return NULL;
#else
    {
	char *end;
	// the codeset is between '.' and '@'.
	if (IS_NULL(enc = strchr (r->h->locale, '.')))
	    return NULL;

	if (!IS_NULL (end = strchr (r->h->locale, '@')))
	    end[0] = '\0';

	enc ++;
    }
#endif

    if ((char) 0 == *enc)
	return NULL;

    return enc;
}