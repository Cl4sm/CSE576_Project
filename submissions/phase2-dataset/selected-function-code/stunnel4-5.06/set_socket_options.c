    SOCK_OPT *ptr;
    extern SOCK_OPT sock_opts[];
    static char *type_str[3]={"accept", "local", "remote"};
    int opt_size;
    int retval=0; /* no error found */

    for(ptr=sock_opts; ptr->opt_str; ptr++) {
        if(!ptr->opt_val[type])
            continue; /* default */
        switch(ptr->opt_type) {
        case TYPE_LINGER:
            opt_size=sizeof(struct linger);
            break;
        case TYPE_TIMEVAL:
            opt_size=sizeof(struct timeval);
            break;
        case TYPE_STRING:
            opt_size=strlen(ptr->opt_val[type]->c_val)+1;
            break;
        default:
            opt_size=sizeof(int);
        }
        if(setsockopt(s, ptr->opt_level, ptr->opt_name,
                (void *)ptr->opt_val[type], opt_size)) {
            if(get_last_socket_error()==S_EOPNOTSUPP) {
                /* most likely stdin/stdout or AF_UNIX socket */
                s_log(LOG_DEBUG,
                    "Option %s not supported on %s socket",
                    ptr->opt_str, type_str[type]);
            } else {
                sockerror(ptr->opt_str);
                retval=-1; /* failed to set this option */
            }
        }
#ifdef DEBUG_FD_ALLOC
        else {
            s_log(LOG_DEBUG, "Option %s set on %s socket",
                ptr->opt_str, type_str[type]);
        }
#endif /* DEBUG_FD_ALLOC */
    }
    return retval; /* returns 0 when all options succeeded */
}
