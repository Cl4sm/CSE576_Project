void
rxvt_close_all_tabs( rxvt_t *r)
{
    static struct timeval   lastRequest = {0, 0};
    struct timeval	    now;
    int			    i;

    for( i=LTAB(r); i >=0; i-- )
    {
	if( PVTS(r, i)->dead )
	    rxvt_remove_page( r, i );

	else
	{
	    PVTS(r, i)->holdOption = 0;
	    kill( PVTS(r, i)->cmd_pid, SIGHUP );
	}
    }

    gettimeofday( &now, NULL );
    if( lastRequest.tv_sec != 0 && now.tv_sec - lastRequest.tv_sec < 5 )
	/* Second request within 5 seconds. Kill mrxvt */
	rxvt_clean_exit(r);

    else
    {
	lastRequest = now;

	/* Just in case the processes don't exit on SIGHUP, warn the user */
	signal( SIGALRM, rxvt_alarm_signal );
	alarm( 2 );
    }
}