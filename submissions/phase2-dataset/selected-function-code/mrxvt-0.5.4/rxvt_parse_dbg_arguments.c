void
rxvt_parse_dbg_arguments (int argc, const char* const* argv)
{
    int	    i;

    for (i = 0; i < argc - 1; i ++)
    {
	if (0 == STRCASECMP("-dmask", argv[i]))
	{
	    if (!rxvt_dbg_parse_masks (argv[i+1]))
	    {
		rxvt_dbg_usage (argv[0]);
		exit (1);
	    }
	    i ++; /* skip arguments to -dm option */
	}

	if (0 == STRCASECMP("-dlevel", argv[i]))
	{
	    if (!rxvt_dbg_parse_level (argv[i+1]))
	    {
		rxvt_dbg_usage (argv[0]);
		exit (1);
	    }
	    i ++; /* skip arguments to -dl option */
	}
    }

#ifdef DEBUG
    fprintf (stderr, "Debug mask: 0x%08x, debug level: %d\n", g_dbg_mask, g_dbg_level);
#endif
}