int
main(int argc, const char *const *argv)
{
    rxvt_t*	rxvt_vars;

    /*
     * Save and then give up any super-user privileges immediately after program
     * starts. If we need privileges in any area then we must specifically
     * request it.
     *
     * We should only need to be root in these cases:
     *
     *	    1.  write utmp entries on some systems
     *	    2.  chown tty on some systems
     */
    rxvt_privileges(SAVE);
    rxvt_privileges(IGNORE);

    /*
     * Initialize debug level and debug mask. Must do it first in order
     * to debug memory management system.
     */
    rxvt_parse_dbg_arguments (argc, argv);

#ifdef OUR_MALLOC
    /* initialize our memory management system */
    rxvt_mem_init ();
#endif

    if ((rxvt_vars = rxvt_init(argc, argv)) == NULL)
	return EXIT_FAILURE;

    rxvt_main_loop(rxvt_vars);	/* main processing loop */

#ifdef OUR_MALLOC
    rxvt_mem_exit ();
#endif

    return EXIT_SUCCESS;
}