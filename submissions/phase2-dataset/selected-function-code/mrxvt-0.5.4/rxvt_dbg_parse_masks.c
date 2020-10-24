static int
rxvt_dbg_parse_masks (const char* maskstring)
{
    int		mask = 0;
    const char*	ptr;

    if ('0' == maskstring[0] &&
	('x' == maskstring[1] || 'X' == maskstring[1]))
    {
	for (ptr = maskstring+2; *ptr; ptr++)
	{
	    if (*ptr < '0' || *ptr > '9')
		return 0;   /* invalid hex number */
	    mask = (mask << 4) + (*ptr - '0');
	}
	if (ptr - maskstring > 32)
	    return 0;	    /* mask hex too long */
    }
    else
    {
	const char* pcoma;
	int	    len;

	ptr = pcoma = maskstring;

	do
	{
	    while (*pcoma && ',' != *pcoma)
		pcoma ++;

	    len = pcoma - ptr;
	    if (0 == len)
		return 0;	/* a single ',' without mask name */

	    if (len == sizeof ("COMMAND") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_COMMAND], ptr, len))
		mask |= DBG_COMMAND;
	    else if (len == sizeof ("SCREEN") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_SCREEN], ptr, len))
		mask |= DBG_SCREEN;
	    else if (len == sizeof ("PTYTTY") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_PTYTTY], ptr, len))
		mask |= DBG_PTYTTY;
	    else if (len == sizeof ("INIT") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_INIT], ptr, len))
		mask |= DBG_INIT;
	    else if (len == sizeof ("MAIN") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_MAIN], ptr, len))
		mask |= DBG_MAIN;
	    else if (len == sizeof ("LOGGING") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_LOGGING], ptr, len))
		mask |= DBG_LOGGING;
	    else if (len == sizeof ("MACROS") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_MACROS], ptr, len))
		mask |= DBG_MACROS;
	    else if (len == sizeof ("MENUBAR") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_MENUBAR], ptr, len))
		mask |= DBG_MENUBAR;
	    else if (len == sizeof ("TABBAR") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_TABBAR], ptr, len))
		mask |= DBG_TABBAR;
	    else if (len == sizeof ("SCROLLBAR") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_SCROLLBAR], ptr, len))
		mask |= DBG_SCROLLBAR;
	    else if (len == sizeof ("IMAGES") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_IMAGES], ptr, len))
		mask |= DBG_IMAGES;
	    else if (len == sizeof ("PIXMAP") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_PIXMAP], ptr, len))
		mask |= DBG_PIXMAP;
	    else if (len == sizeof ("TRANSPARENT") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_TRANSPARENT], ptr, len))
		mask |= DBG_TRANSPARENT;
	    else if (len == sizeof ("ENCODING") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_ENCODING], ptr, len))
		mask |= DBG_ENCODING;
	    else if (len == sizeof ("GKRELOT") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_GKRELOT], ptr, len))
		mask |= DBG_GKRELOT;
	    else if (len == sizeof ("MEMORY") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_MEMORY], ptr, len))
		mask |= DBG_MEMORY;
	    else if (len == sizeof ("SESSION") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_SESSION], ptr, len))
		mask |= DBG_SESSION;
	    else if (len == sizeof ("STRING") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_STRING], ptr, len))
		mask |= DBG_STRING;
	    else if (len == sizeof ("RESOURCE") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_RESOURCE], ptr, len))
		mask |= DBG_RESOURCE;
	    else if (len == sizeof ("XFTACS") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_XFTACS], ptr, len))
		mask |= DBG_XFTACS;
	    else if (len == sizeof ("MISC") -1 &&
		0 == STRNCASECMP (dbg_mask_name[DBG_IDX_MISC], ptr, len))
		mask |= DBG_MISC;
	    else if (len == sizeof ("ALL") -1 &&
		0 == STRNCASECMP ("ALL", ptr, len))
		mask |= 0xffffffff;
	    else
	    {
		fprintf (stderr, "Error: unknown debug mask name %s\n", ptr);
		return 0;	/* invalid mask name */
	    }

	    if (!*pcoma)
		break;

	    pcoma ++;
	    ptr = pcoma;
	}
	while (*ptr);
    }

    g_dbg_mask = mask;
    return 1;
}