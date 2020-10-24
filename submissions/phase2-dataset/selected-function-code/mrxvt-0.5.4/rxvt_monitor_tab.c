void
rxvt_monitor_tab(rxvt_t* r,int i)
{
    struct timeval  tp;
    struct timeval  monitor_timeout_time;

    short  execute_action = 0;
    int	   monitor_timeout = 2000;
    
    /* return, if no monitoring is activated */
    if ((IS_NULL(&PVTS(r, i)->monitor_tab)) || 
	(PVTS(r, i)->monitor_tab == TAB_MON_OFF) ||
	(PVTS(r, i)->monitor_tab == TAB_MON_NOTIFICATION))
	return;

    monitor_timeout_time = PVTS(r, i)->monitor_start;
    /* set configured monitor_timeout milliseconds , if configured */
    if( r->h->rs[Rs_monitorTimeout] )
	monitor_timeout = atoi( r->h->rs[Rs_monitorTimeout] );

    monitor_timeout_time.tv_sec += (int) monitor_timeout/1000;
    monitor_timeout_time.tv_usec +=
	(monitor_timeout - (((int) monitor_timeout/1000) * 1000)) * 1000;

    /* get current epoch time */
    gettimeofday( &tp, NULL);

    /* monitor-type "AUTO" : determine which type of monitoring is needed */
    if ((PVTS(r, i)->monitor_tab == TAB_MON_AUTO) && 
	(timercmp(&monitor_timeout_time,&tp, <)))
    {
	if(PVTS(r, i)->monitor_nbytes_read > 0)
	{
	    PVTS(r, i)->monitor_tab = TAB_MON_INACTIVITY;
	    rxvt_msg (DBG_INFO, DBG_MACROS,
		    "Macro MonitorTab: decided to monitor inactivity on tab %i",
		    i);
	}
	else
	{
	    PVTS(r, i)->monitor_tab = TAB_MON_ACTIVITY;
	    rxvt_msg (DBG_INFO, DBG_MACROS,  
	       "Macro MonitorTab: decided to monitor activity on tab %i", i);
	}
	PVTS(r, i)->monitor_nbytes_read = 0 ;
	PVTS(r, i)->monitor_start = tp;
    }
    /* monitor-type "INACTIVITY" : detect inactivity */
    else if ((PVTS(r, i)->monitor_tab == TAB_MON_INACTIVITY) &&
	    (timercmp(&monitor_timeout_time,&tp, <)))
    {
	/* inactivity detected */
	if (PVTS( r, i)->monitor_nbytes_read == 0)
	{
	    rxvt_msg (DBG_INFO, DBG_MACROS,
		    "Macro MonitorTab: detected inactivity on tab %i", i);
	    execute_action = 1;
	}
	/* activity detected, restarting monitoring */
	else
	{
	    rxvt_msg (DBG_DEBUG, DBG_MACROS,
		    "Macro MonitorTab: NOT detected inactivity on tab %i / %i ",
		    i, PVTS(r,i)->monitor_nbytes_read);
	    PVTS(r, i)->monitor_start = tp;
	    PVTS(r, i)->monitor_nbytes_read = 0;
	}
    }
    /* monitor-type "ACTIVITY" : detect activity */
    else if ((PVTS(r, i)->monitor_tab == TAB_MON_ACTIVITY) &&
	     (PVTS( r, i)->monitor_nbytes_read != 0))
    {
	rxvt_msg (DBG_INFO, DBG_MACROS,
		"Macro MonitorTab: detected activity on tab %i", i);
	execute_action = 1;
    }

    /* stop execution of this function if no activity/inactivity 
     * needs to be notified 
     */
    if (execute_action == 0)
	return;

    const int   maxLen = 1024;
    char        expstr[maxLen];

    /* execute a command if configured */
    if( r->h->rs[Rs_monitorCommand] && *r->h->rs[Rs_monitorCommand] )
    {
	/* interpolate percent arguments */
	rxvt_percent_interpolate( r, i, (char *) r->h->rs[Rs_monitorCommand],
	    STRLEN(r->h->rs[Rs_monitorCommand]), (char*) expstr, maxLen );

	rxvt_async_exec( r, expstr );
    }

    /* ding - ring the system bell */
    rxvt_scr_bell(r,i);

    /* highlight the tabbar */
    rxvt_tabbar_highlight_tab (r, i, False);

    /* mark tab as active, if it is not the active tab */
    if (ATAB(r) != i)
    {
	PVTS(r, i)->monitor_tab = TAB_MON_NOTIFICATION;
    }
    else
    {
	PVTS(r, i)->monitor_tab = TAB_MON_OFF;
    }

#ifdef BACKGROUND_IMAGE
    if( r->tabBar.hasPixmap  && ISSET_OPTION(r, Opt_tabPixmap))
    {
	PVTS(r, i)->monitor_tab = TAB_MON_OFF;
	rxvt_dbgmsg ((DBG_INFO, DBG_TABBAR,
		    "Disabling background filling, background image is "
		    "activated"));
    }
#endif
#ifdef TRANSPARENT
    if ( ( r->h->am_transparent || r->h->am_pixmap_trans ) &&
	ISSET_OPTION(r, Opt_transparent_tabbar))
    {
	PVTS(r, i)->monitor_tab = TAB_MON_OFF;
	rxvt_dbgmsg ((DBG_INFO, DBG_TABBAR,
		    "Disabling background filling, option 'transparentTabbar'"
		    "is enabled"));
    }
#endif
    rxvt_tabbar_expose (r, NULL);
}