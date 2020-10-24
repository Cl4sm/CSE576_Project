void
rxvt_set_default_font_x11 (rxvt_t* r)
{
    register int    i;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_ENCODING, "%s\n", __func__));

    /* Set default fonts */
    def_fontName = (char**) nfont_list[r->encoding_method].font;

#ifdef MULTICHAR_SET
    switch (r->encoding_method)	{
    case ENC_SJIS :
	def_mfontName = (char**) mfont_list[ENC_SJIS].mfont;
	break;
    case ENC_EUCJ :
	def_mfontName = (char**) mfont_list[ENC_EUCJ].mfont;
	break;
    case ENC_GB   :
	def_mfontName = (char**) mfont_list[ENC_GB].mfont;
	break;
    case ENC_GBK  :
	def_mfontName = (char**) mfont_list[ENC_GBK].mfont;
	break;
    case ENC_GB18030  :
	def_mfontName = (char**) mfont_list[ENC_GB18030].mfont;
	break;
    case ENC_BIG5 :
	def_mfontName = (char**) mfont_list[ENC_BIG5].mfont;
	break;
    case ENC_EUCKR:
	def_mfontName = (char**) mfont_list[ENC_EUCKR].mfont;
	break;
    default:
	def_mfontName = (char**) mfont_list[r->encoding_method].mfont;
	break;
    }

    /* Found no mfont, fall back to ISO8859-X font */
    if (IS_NULL(def_mfontName[0]))  {
	for (i = 0; i < MAX_NFONTS; i ++)   {
	    char*   ptr = rxvt_malloc (STRLEN(isofont[i])+4);
	    if (r->encoding_method >= ENC_ISO8859_1 &&
		r->encoding_method <= ENC_ISO8859_15)
		sprintf (ptr, isofont[i],
		    r->encoding_method - ENC_ISO8859_1 + 1);
	    else
		sprintf (ptr, isofont[i], 1);
	    def_mfontName[i] = ptr;
	}
    }
#endif

    /* Found no font, fall back to ISO8859-X font */
    if (IS_NULL(def_fontName[0]))   {
	for (i = 0; i < MAX_NFONTS; i ++)   {
	    char*   ptr = rxvt_malloc (STRLEN(isofont[i])+4);
#ifdef MULTICHAR_SET
	    if (r->encoding_method >= ENC_ISO8859_1 &&
		r->encoding_method <= ENC_ISO8859_15)
		sprintf (ptr, isofont[i],
		    r->encoding_method - ENC_ISO8859_1 + 1);
	    else
#endif
	    sprintf (ptr, isofont[i], 1);
	    def_fontName[i] = ptr;
	}
    }

    /* Overrided by -km option or X resources */
    for (i = 0; i < MAX_NFONTS; i ++)	{
	if (IS_NULL(r->h->rs[Rs_font +i]))
	    r->h->rs[Rs_font +i] = def_fontName[i];
#ifdef MULTICHAR_SET
	if (IS_NULL(r->h->rs[Rs_mfont +i]))
	    r->h->rs[Rs_mfont +i] = def_mfontName[i];
#endif
    }
}