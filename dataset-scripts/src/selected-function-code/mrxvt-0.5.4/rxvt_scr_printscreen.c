 */
/* EXTPROTO */
void
rxvt_scr_printscreen(rxvt_t* r, int page, int fullhist, int pretty,
	int linecont, const char *pipeName )
{
#ifdef PRINTPIPE
    int		row, col, nrows, row_offset;
    text_t	*txt;
    rend_t	*rnd;

    FILE*	fd;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_scr_printscreen( r, %d, %d, %d, %s )\n", page, fullhist, pretty, pipeName ));

    if ( ( fd = rxvt_popen_printer( r, pipeName ) ) == NULL )
	return;

    nrows	= r->TermWin.nrow;
    row_offset	= SVLINES;
    if (!fullhist)
	row_offset -= VSTART;
    else
    {
	nrows += PVTS(r, page)->nscrolled;
	row_offset -= PVTS(r, page)->nscrolled;
    }

    for( row=0; row < nrows && !ferror( fd ); row++ )
    {
	int lineEnd;

	txt = PSCR( r, page ).text[ row + row_offset ];
	rnd = PSCR( r, page ).rend[ row + row_offset ];

	if( linecont && PSCR(r, page).tlen[row+row_offset] == -1 )
	    /* Line continues over. Don't trim trailing spaces */
	    lineEnd = r->TermWin.ncol - 1;
	else
	{
	    /* Trim trailing spaces */
	    for(
		 lineEnd = r->TermWin.ncol - 1;
		 lineEnd >= 0 && isspace( txt[lineEnd] );
		 lineEnd--
	       )
	    ;
	}

	if( pretty )
	{
	    /* Print colors as escape sequences */

	    for( col=0; col <= lineEnd; )
	    {
		char	escsq[32];  /* Buffer to hold the escape sequence. 29
				       bytes are enough. */
		char	*s, *t;
		int	start = col;
		int	color;

		rend_t	rend = rnd[start];

		/* Get longest string with constant rendition attrs */

		do
		  {
		    col++;
		  }
		while( col <= lineEnd && rnd[col] == rend );


		t = s = escsq + sprintf( escsq, "\e[" );

		if( rend & RS_Bold )	    s += sprintf( s, "1;" );
		if( rend & RS_Uline )	    s += sprintf( s, "4;" );
		if( rend & RS_Blink )	    s += sprintf( s, "5;" );
		if( rend & RS_RVid )	    s += sprintf( s, "7;" );

		color = GET_BASEFG( rend );
		if( color != Color_fg )
		    s = escSetColor( s, color, 1 );

		color = GET_BASEBG( rend );
		if( color != Color_bg )
		    s = escSetColor( s, color, 0 );

		if( s != t )
		{
		    /*
		     * Some esc seq has been set. Null terminate and Replace
		     * trailing ';' with 'm'
		     */
		    *(s--)  = '\0';
		    *s	    = 'm';

		    fprintf( fd, "%s%.*s\e[0m", escsq, col-start, txt+start );
		}

		else
		    fprintf( fd, "%.*s", col - start, txt + start );

	    }
	} /* if( pretty ) */

	else
	    /* Vanilla text */
	    fprintf( fd, "%.*s", (lineEnd + 1), txt );

	if( !linecont || PSCR(r, page).tlen[row+row_offset] != -1 )
	    fputc( '\n', fd );

    } /* for( row ... ) */

    rxvt_pclose_printer(fd);