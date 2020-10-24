static int
rxvt_dbg_parse_level (const char* levelstring)
{
    int	    level;

    if ((char) 0 == *levelstring)
	return 0;

    if ('0' <= *levelstring && *levelstring <= '9')
	level = atoi (levelstring);
    else
    {
	for (level = 0; level < DBG_LEVEL_MAX; level ++)
	    if (0 == STRCASECMP (dbg_level_name[level], levelstring))
		break;
    }
    if (level < 0 || level >= DBG_LEVEL_MAX)
	return 0;

    g_dbg_level = level;

    return 1;
}