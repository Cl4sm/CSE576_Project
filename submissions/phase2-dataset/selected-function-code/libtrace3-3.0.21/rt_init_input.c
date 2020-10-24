static int rt_init_input(libtrace_t *libtrace) {
        char *scan;
        char *uridata = libtrace->uridata;

	rt_init_format_data(libtrace);

	/* If the user specifies "rt:" then assume localhost and the default
	 * port */	
        if (strlen(uridata) == 0) {
                RT_INFO->hostname =
                        strdup("localhost");
                RT_INFO->port =
                        COLLECTOR_PORT;
        } else {
                /* If the user does not specify a port, assume the default 
		 * port */
		if ((scan = strchr(uridata,':')) == NULL) {
                        RT_INFO->hostname =
                                strdup(uridata);
                        RT_INFO->port =
                                COLLECTOR_PORT;
                } else {
                        RT_INFO->hostname =
                                (char *)strndup(uridata,
                                                (size_t)(scan - uridata));
                        RT_INFO->port =
                                atoi(++scan);
                }
        }

	return 0;
}