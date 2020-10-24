int
rxvt_percent_interpolate( rxvt_t *r, int page,
	const char *src, int len, char *dst, int maxLen )
{
    int	i=0,	/* Unexpanded string index */
	j=0;	/* Expanded string index */

    rxvt_dbgmsg ((DBG_DEBUG, DBG_MISC, "rxvt_percent_interpolate( r, %d, %s, %d, %s, %d )\n", page, src, len, "dst", maxLen));

    /* Must only get here for a valid tab */
    assert( page >=0 && page <= LTAB(r) );
    assert( NOT_NULL( PVTS( r, page ) ) && PVTS( r, page )->vts_idx != -1 );

    while( i < len-1 && j < maxLen-1 )
    {
	if( src[i] == '%' )
	{
	    switch( src[++i] )
	    {
		case '%':
		    /* Copy % over */
		    dst[j++] = src[i++];
		    break;

		case 'n':
		    /* Active tab number */
		    j += snprintf( dst + j, maxLen - j, "%d", page+1 );
		    i ++;
		    break;

		case 't':
		    /* Active tab title */
		    j += snprintf( dst + j, maxLen -j,
			    "%s", PVTS(r, page)->tab_title );
		    i ++;
		    break;

		case 'S':
		    /* Exit status of dead processes */
		    if( PVTS( r, page )->dead )
			j += snprintf( dst + j, maxLen - j, "%d",
				WEXITSTATUS( PVTS( r, page )->status ) );
		    else
			dst[ j++ ] = src[ i ];

		    i++;
		    break;

		case 'N':
		    /* Normal / abnormal exit status of dead processes */
		    if( PVTS( r, page )->dead )
			j += snprintf( dst + j, maxLen - j, "%s",
				WIFEXITED( PVTS( r, page )->status )
				    ? "normally" : "abnormally" );
		    else
			dst[ j++ ] = src[ i ];

		    i ++;
		    break;

		case 's':
		    /*
		     * Selection. TODO Also paste selection if it is not
		     * owned by mrxvt.
		     */
		    if( NOT_NULL( r->selection.text ) )
			j += snprintf( dst + j, maxLen -j,
				"%s", r->selection.text );
		    i++;
		    break;

		case 'p':
		    /* Pid of process in current tab */
		    j += snprintf( dst + j, maxLen - j, "%d",
			    PVTS(r, page)->cmd_pid );
		    i++;
		    break;

		case 'P':
		    /* PID of mrxvt */
		    j += snprintf( dst + j, maxLen - j, "%d", getpid() );
		    i++;
		    break;

		case 'G':
		    /* Global tab number (plus 1) */
		    j += snprintf( dst + j, maxLen - j, "%d",
			    PVTS( r, page )->globalTabNum + 1 );
		    i ++;
		    break;

		case 'T':
		    /* # tabs created so far */
		    j += snprintf( dst + j, maxLen - j, "%d",
			    r->ntabs + 1 );
		    i ++;
		    break;

		default:
		    rxvt_msg (DBG_ERROR, DBG_MISC, "Unrecognized flag %%%c in '%s'", src[i++], src );
		    break;
	    }
	}
	else
	    dst[j++] = src[i++];
    }

    /* Copy last char over */
    if( i == len-1 && j < maxLen-1 )
	dst[j++] = src[i++];

    /* NULL terminate dst */
    if( j > maxLen - 1 )
	j = maxLen - 1;
    if( j == 0 || dst[j-1] )
	dst[j++] = 0;

    /* % expansion done. Copy the string and length over */
    return j;
}