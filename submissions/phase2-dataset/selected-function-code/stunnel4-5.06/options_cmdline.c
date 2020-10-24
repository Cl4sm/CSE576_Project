    CONF_TYPE type=CONF_FILE;

#ifdef USE_WIN32
    (void)parameter; /* skip warning about unused parameter */
#endif
    if(!name) {
        /* leave the previous value of configuration_file */
    } else if(!strcasecmp(name, "-help")) {
        parse_global_option(CMD_HELP, NULL, NULL);
        parse_service_option(CMD_HELP, NULL, NULL, NULL);
        log_flush(LOG_MODE_INFO);
        return 1;
    } else if(!strcasecmp(name, "-version")) {
        parse_global_option(CMD_DEFAULT, NULL, NULL);
        parse_service_option(CMD_DEFAULT, NULL, NULL, NULL);
        log_flush(LOG_MODE_INFO);
        return 1;
    } else if(!strcasecmp(name, "-sockets")) {
        print_socket_options();
        log_flush(LOG_MODE_INFO);
        return 1;
    } else if(!strcasecmp(name, "-options")) {
        print_ssl_options();
        log_flush(LOG_MODE_INFO);
        return 1;
    } else
#ifndef USE_WIN32
    if(!strcasecmp(name, "-fd")) {
        if(!parameter) {
            s_log(LOG_ERR, "No file descriptor specified");
            print_syntax();
            return 1;
        }
        configuration_file=parameter;
        type=CONF_FD;
    } else
#endif
        configuration_file=name;
    configuration_file=str_dup(configuration_file);
    str_detach(configuration_file); /* do not track this allocation */

    return options_parse(type);
}
