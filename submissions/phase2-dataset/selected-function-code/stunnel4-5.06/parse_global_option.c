    char *tmpstr;
#ifndef USE_WIN32
    struct group *gr;
    struct passwd *pw;
#endif

    if(cmd==CMD_DEFAULT || cmd==CMD_HELP) {
        s_log(LOG_NOTICE, " ");
        s_log(LOG_NOTICE, "Global options:");
    }

    /* chroot */
#ifdef HAVE_CHROOT
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.chroot_dir=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "chroot"))
            break;
        new_global_options.chroot_dir=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = directory to chroot stunnel process", "chroot");
        break;
    }
#endif /* HAVE_CHROOT */

    /* compression */
#ifndef OPENSSL_NO_COMP
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.compression=COMP_NONE;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "compression"))
            break;
        if(SSLeay()>=0x00908051L && !strcasecmp(arg, "deflate"))
            return "deflate compression is not really implemented in OpenSSL";
        else if(!strcasecmp(arg, "zlib"))
            return "zlib compression is disabled in the Debian package of OpenSSL";
        else if(!strcasecmp(arg, "rle"))
            /**
             * OpenSSL RLE compression has never really been properly
             * implemented.  The routines that are in place are
             * non-functional for two reasons:
             * - they do not really compress anything, just copy the data
             * - they do not handle the empty (zero-length) fragment added
             *   in OpenSSL 0.9.6d, so the first attempt to send data over
             *   the SSL connection fails and aborts the connection
             */
            return "RLE compression is not implemented in OpenSSL";
        else
            return "Specified compression type is not available";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = compression type",
            "compression");
        break;
    }
#endif /* OPENSSL_NO_COMP */

    /* debug */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.debug_level=LOG_NOTICE;
#if !defined (USE_WIN32) && !defined (__vms)
        new_global_options.facility=LOG_DAEMON;
#endif
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "debug"))
            break;
        if(parse_debug_level(arg))
            return "Illegal debug argument";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
#if !defined (USE_WIN32) && !defined (__vms)
        s_log(LOG_NOTICE, "%-22s = %s", "debug", "daemon.notice");
#else
        s_log(LOG_NOTICE, "%-22s = %s", "debug", "notice");
#endif
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = [facility].level (e.g. daemon.info)", "debug");
        break;
    }

    /* EGD */
    switch(cmd) {
    case CMD_BEGIN:
#ifdef EGD_SOCKET
        new_global_options.egd_sock=EGD_SOCKET;
#else
        new_global_options.egd_sock=NULL;
#endif
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "EGD"))
            break;
        new_global_options.egd_sock=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
#ifdef EGD_SOCKET
        s_log(LOG_NOTICE, "%-22s = %s", "EGD", EGD_SOCKET);
#endif
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = path to Entropy Gathering Daemon socket", "EGD");
        break;
    }

#ifdef HAVE_OSSL_ENGINE_H

    /* engine */
    switch(cmd) {
    case CMD_BEGIN:
        engine_reset_list();
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "engine"))
            break;
        if(!strcasecmp(arg, "auto"))
            return engine_auto();
        else
            return engine_open(arg);
    case CMD_END:
        engine_next();
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = auto|engine_id",
            "engine");
        break;
    }

    /* engineCtrl */
    switch(cmd) {
    case CMD_BEGIN:
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "engineCtrl"))
            break;
        tmpstr=strchr(arg, ':');
        if(tmpstr)
            *tmpstr++='\0';
        return engine_ctrl(arg, tmpstr);
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = cmd[:arg]",
            "engineCtrl");
        break;
    }

    /* engineDefault */
    switch(cmd) {
    case CMD_BEGIN:
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "engineDefault"))
            break;
        return engine_default(arg);
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = TASK_LIST",
            "engineDefault");
        break;
    }

#endif

    /* fips */
    switch(cmd) {
    case CMD_BEGIN:
#ifdef USE_FIPS
        new_global_options.option.fips=0;
#endif /* USE_FIPS */
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "fips"))
            break;
#ifdef USE_FIPS
        if(!strcasecmp(arg, "yes"))
            new_global_options.option.fips=1;
        else if(!strcasecmp(arg, "no"))
            new_global_options.option.fips=0;
        else
            return "Argument should be either 'yes' or 'no'";
#else
        if(strcasecmp(arg, "no"))
            return "FIPS support is not available";
#endif /* USE_FIPS */
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
#ifdef USE_FIPS
        s_log(LOG_NOTICE, "%-22s = yes|no FIPS 140-2 mode",
            "fips");
#endif /* USE_FIPS */
        break;
    }

    /* foreground */
#ifndef USE_WIN32
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.option.foreground=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "foreground"))
            break;
        if(!strcasecmp(arg, "yes"))
            new_global_options.option.foreground=1;
        else if(!strcasecmp(arg, "no"))
            new_global_options.option.foreground=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no foreground mode (don't fork, log to stderr)",
            "foreground");
        break;
    }
#endif

#ifdef ICON_IMAGE

    /* iconActive */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.icon[ICON_ACTIVE]=load_icon_default(ICON_ACTIVE);
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "iconActive"))
            break;
        if(!(new_global_options.icon[ICON_ACTIVE]=load_icon_file(arg)))
            return "Failed to load the specified icon";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = icon when connections are established", "iconActive");
        break;
    }

    /* iconError */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.icon[ICON_ERROR]=load_icon_default(ICON_ERROR);
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "iconError"))
            break;
        if(!(new_global_options.icon[ICON_ERROR]=load_icon_file(arg)))
            return "Failed to load the specified icon";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = icon for invalid configuration file", "iconError");
        break;
    }

    /* iconIdle */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.icon[ICON_IDLE]=load_icon_default(ICON_IDLE);
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "iconIdle"))
            break;
        if(!(new_global_options.icon[ICON_IDLE]=load_icon_file(arg)))
            return "Failed to load the specified icon";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = icon when no connections were established", "iconIdle");
        break;
    }

#endif /* ICON_IMAGE */

    /* log */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.log_file_mode=FILE_MODE_APPEND;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "log"))
            break;
        if(!strcasecmp(arg, "append"))
            new_global_options.log_file_mode=FILE_MODE_APPEND;
        else if(!strcasecmp(arg, "overwrite"))
            new_global_options.log_file_mode=FILE_MODE_OVERWRITE;
        else
            return "Argument should be either 'append' or 'overwrite'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = append|overwrite log file",
            "log");
        break;
    }

    /* output */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.output_file=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "output"))
            break;
        new_global_options.output_file=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = file to append log messages", "output");
        break;
    }

    /* pid */
#ifndef USE_WIN32
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.pidfile=PIDFILE;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "pid"))
            break;
        if(arg[0]) /* is argument not empty? */
            new_global_options.pidfile=str_dup(arg);
        else
            new_global_options.pidfile=NULL; /* empty -> do not create a pid file */
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %s", "pid", PIDFILE);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = pid file (empty to disable creating)", "pid");
        break;
    }
#endif

    /* RNDbytes */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.random_bytes=RANDOM_BYTES;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "RNDbytes"))
            break;
        new_global_options.random_bytes=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal number of bytes to read from random seed files";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %d", "RNDbytes", RANDOM_BYTES);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = bytes to read from random seed files", "RNDbytes");
        break;
    }

    /* RNDfile */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.rand_file=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "RNDfile"))
            break;
        new_global_options.rand_file=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
#ifdef RANDOM_FILE
        s_log(LOG_NOTICE, "%-22s = %s", "RNDfile", RANDOM_FILE);
#endif
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = path to file with random seed data", "RNDfile");
        break;
    }

    /* RNDoverwrite */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.option.rand_write=1;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "RNDoverwrite"))
            break;
        if(!strcasecmp(arg, "yes"))
            new_global_options.option.rand_write=1;
        else if(!strcasecmp(arg, "no"))
            new_global_options.option.rand_write=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = yes", "RNDoverwrite");
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no overwrite seed datafiles with new random data",
            "RNDoverwrite");
        break;
    }

#ifndef USE_WIN32
    /* service */
    switch(cmd) {
    case CMD_BEGIN:
        new_service_options.servname=str_dup("stunnel");
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "service"))
            break;
        new_service_options.servname=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = service name", "service");
        break;
    }
#endif

#ifndef USE_WIN32
    /* setgid */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.gid=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "setgid"))
            break;
        gr=getgrnam(arg);
        if(gr) {
            new_global_options.gid=gr->gr_gid;
            return NULL; /* OK */
        }
        new_global_options.gid=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal GID";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = groupname for setgid()", "setgid");
        break;
    }
#endif

#ifndef USE_WIN32
    /* setuid */
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.uid=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "setuid"))
            break;
        pw=getpwnam(arg);
        if(pw) {
            new_global_options.uid=pw->pw_uid;
            return NULL; /* OK */
        }
        new_global_options.uid=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal UID";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = username for setuid()", "setuid");
        break;
    }
#endif

    /* socket */
    switch(cmd) {
    case CMD_BEGIN:
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "socket"))
            break;
        if(parse_socket_option(arg))
            return "Illegal socket option";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = a|l|r:option=value[:value]", "socket");
        s_log(LOG_NOTICE, "%25sset an option on accept/local/remote socket", "");
        break;
    }

    /* syslog */
#ifndef USE_WIN32
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.option.syslog=1;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "syslog"))
            break;
        if(!strcasecmp(arg, "yes"))
            new_global_options.option.syslog=1;
        else if(!strcasecmp(arg, "no"))
            new_global_options.option.syslog=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no send logging messages to syslog",
            "syslog");
        break;
    }
#endif

    /* taskbar */
#ifdef USE_WIN32
    switch(cmd) {
    case CMD_BEGIN:
        new_global_options.option.taskbar=1;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "taskbar"))
            break;
        if(!strcasecmp(arg, "yes"))
            new_global_options.option.taskbar=1;
        else if(!strcasecmp(arg, "no"))
            new_global_options.option.taskbar=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = yes", "taskbar");
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no enable the taskbar icon", "taskbar");
        break;
    }
#endif

    if(cmd==CMD_EXEC)
        return option_not_found;

    if(cmd==CMD_END) {
        /* FIPS needs to be initialized as early as possible */
        if(ssl_configure(&new_global_options)) /* configure global SSL settings */
            return "Failed to initialize SSL";
    }
    return NULL; /* OK */
}
