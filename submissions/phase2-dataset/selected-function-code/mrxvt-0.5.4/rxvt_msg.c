int
rxvt_msg (uint32_t level, uint32_t mask, const char* fmt, ...)
{

    /* print fatal/error message regardless of its category */
    if (level <= DBG_ERROR ||
	/* print message unless its category and level are defined */
	(g_dbg_level >= level && mask & g_dbg_mask))
    {
	int	len;
	va_list	ap;

	va_start (ap, fmt);
	len = vfprintf (stderr, fmt, ap);
	va_end (ap);
	
        /* add a newline if last character of fmt is not a newline */
        if( fmt[ STRLEN(fmt)-1 ] != '\n' )
	{
             fputc( '\n', stderr );
	     len++;
	}
        return (len);
    }
    return 0;
}