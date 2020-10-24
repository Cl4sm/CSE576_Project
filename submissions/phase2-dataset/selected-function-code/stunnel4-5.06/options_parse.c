    DISK_FILE *df;
    char line_text[CONFLINELEN], *errstr;
    char config_line[CONFLINELEN], *config_opt, *config_arg;
    int line_number, i;
    SERVICE_OPTIONS *section, *new_section;
#ifndef USE_WIN32
    int fd;
    char *tmpstr;
#endif

    s_log(LOG_NOTICE, "Reading configuration from %s %s",
        type==CONF_FD ? "descriptor" : "file", configuration_file);
#ifndef USE_WIN32
    if(type==CONF_FD) { /* file descriptor */
        fd=strtol(configuration_file, &tmpstr, 10);
        if(tmpstr==configuration_file || *tmpstr) { /* not a number */
            s_log(LOG_ERR, "Invalid file descriptor number");
            print_syntax();
            return 1;
        }
        df=file_fdopen(fd);
    } else
#endif
        df=file_open(configuration_file, FILE_MODE_READ);
    if(!df) {
        s_log(LOG_ERR, "Cannot open configuration file");
        if(type!=CONF_RELOAD)
            print_syntax();
        return 1;
    }

    options_defaults();
    section=&new_service_options;
    line_number=0;
    while(file_getline(df, line_text, CONFLINELEN)>=0) {
        memcpy(config_line, line_text, CONFLINELEN);
        ++line_number;
        config_opt=config_line;
        while(isspace((unsigned char)*config_opt))
            ++config_opt; /* remove initial whitespaces */
        for(i=strlen(config_opt)-1; i>=0 && isspace((unsigned char)config_opt[i]); --i)
            config_opt[i]='\0'; /* remove trailing whitespaces */
        if(config_opt[0]=='\0' || config_opt[0]=='#' || config_opt[0]==';') /* empty or comment */
            continue;
        if(config_opt[0]=='[' && config_opt[strlen(config_opt)-1]==']') { /* new section */
            if(!new_service_options.next) {
                errstr=parse_global_option(CMD_END, NULL, NULL);
                if(errstr) {
                    s_log(LOG_ERR, "Line %d: \"%s\": %s",
                        line_number, line_text, errstr);
                    file_close(df);
                    return 1;
                }
            }
            ++config_opt;
            config_opt[strlen(config_opt)-1]='\0';
            new_section=str_alloc(sizeof(SERVICE_OPTIONS));
            memcpy(new_section, &new_service_options, sizeof(SERVICE_OPTIONS));
            new_section->servname=str_dup(config_opt);
            new_section->session=NULL;
            new_section->next=NULL;
            section->next=new_section;
            section=new_section;
            continue;
        }
        config_arg=strchr(config_line, '=');
        if(!config_arg) {
            s_log(LOG_ERR, "Line %d: \"%s\": No '=' found", line_number, line_text);
            file_close(df);
            return 1;
        }
        *config_arg++='\0'; /* split into option name and argument value */
        for(i=strlen(config_opt)-1; i>=0 && isspace((unsigned char)config_opt[i]); --i)
            config_opt[i]='\0'; /* remove trailing whitespaces */
        while(isspace((unsigned char)*config_arg))
            ++config_arg; /* remove initial whitespaces */
        errstr=parse_service_option(CMD_EXEC, section, config_opt, config_arg);
        if(!new_service_options.next && errstr==option_not_found)
            errstr=parse_global_option(CMD_EXEC, config_opt, config_arg);
        if(errstr) {
            s_log(LOG_ERR, "Line %d: \"%s\": %s", line_number, line_text, errstr);
            file_close(df);
            return 1;
        }
    }
    file_close(df);

    if(new_service_options.next) { /* daemon mode: initialize sections */
        for(section=new_service_options.next; section; section=section->next) {
            s_log(LOG_INFO, "Initializing service [%s]", section->servname);
            errstr=parse_service_option(CMD_END, section, NULL, NULL);
            if(errstr)
                break;
        }
    } else { /* inetd mode: need to initialize global options */
        errstr=parse_global_option(CMD_END, NULL, NULL);
        if(errstr) {
            s_log(LOG_ERR, "Global options: %s", errstr);
            return 1;
        }
        s_log(LOG_INFO, "Initializing inetd mode configuration");
        section=&new_service_options;
        errstr=parse_service_option(CMD_END, section, NULL, NULL);
    }
    if(errstr) {
        s_log(LOG_ERR, "Service [%s]: %s", section->servname, errstr);
        return 1;
    }

    s_log(LOG_NOTICE, "Configuration successful");
    return 0;
}
