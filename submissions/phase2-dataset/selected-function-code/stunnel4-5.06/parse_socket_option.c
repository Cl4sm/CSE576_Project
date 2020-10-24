    int socket_type; /* 0-accept, 1-local, 2-remote */
    char *opt_val_str, *opt_val2_str, *tmpstr;
    SOCK_OPT *ptr;

    if(arg[1]!=':')
        return 1; /* FAILED */
    switch(arg[0]) {
    case 'a':
        socket_type=0; break;
    case 'l':
        socket_type=1; break;
    case 'r':
        socket_type=2; break;
    default:
        return 1; /* FAILED */
    }
    arg+=2;
    opt_val_str=strchr(arg, '=');
    if(!opt_val_str) /* no '='? */
        return 1; /* FAILED */
    *opt_val_str++='\0';
    ptr=sock_opts;
    for(;;) {
        if(!ptr->opt_str)
            return 1; /* FAILED */
        if(!strcmp(arg, ptr->opt_str))
            break; /* option name found */
        ++ptr;
    }
    ptr->opt_val[socket_type]=str_alloc(sizeof(OPT_UNION));
    switch(ptr->opt_type) {
    case TYPE_FLAG:
        if(!strcasecmp(opt_val_str, "yes") || !strcmp(opt_val_str, "1")) {
            ptr->opt_val[socket_type]->i_val=1;
            return 0; /* OK */
        }
        if(!strcasecmp(opt_val_str, "no") || !strcmp(opt_val_str, "0")) {
            ptr->opt_val[socket_type]->i_val=0;
            return 0; /* OK */
        }
        return 1; /* FAILED */
    case TYPE_INT:
        ptr->opt_val[socket_type]->i_val=strtol(opt_val_str, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return 1; /* FAILED */
        return 0; /* OK */
    case TYPE_LINGER:
        opt_val2_str=strchr(opt_val_str, ':');
        if(opt_val2_str) {
            *opt_val2_str++='\0';
            ptr->opt_val[socket_type]->linger_val.l_linger=
                (u_short)strtol(opt_val2_str, &tmpstr, 10);
            if(tmpstr==arg || *tmpstr) /* not a number */
                return 1; /* FAILED */
        } else {
            ptr->opt_val[socket_type]->linger_val.l_linger=0;
        }
        ptr->opt_val[socket_type]->linger_val.l_onoff=
            (u_short)strtol(opt_val_str, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return 1; /* FAILED */
        return 0; /* OK */
    case TYPE_TIMEVAL:
        opt_val2_str=strchr(opt_val_str, ':');
        if(opt_val2_str) {
            *opt_val2_str++='\0';
            ptr->opt_val[socket_type]->timeval_val.tv_usec=strtol(opt_val2_str, &tmpstr, 10);
            if(tmpstr==arg || *tmpstr) /* not a number */
                return 1; /* FAILED */
        } else {
            ptr->opt_val[socket_type]->timeval_val.tv_usec=0;
        }
        ptr->opt_val[socket_type]->timeval_val.tv_sec=strtol(opt_val_str, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return 1; /* FAILED */
        return 0; /* OK */
    case TYPE_STRING:
        if(strlen(opt_val_str)+1>sizeof(OPT_UNION))
            return 1; /* FAILED */
        strcpy(ptr->opt_val[socket_type]->c_val, opt_val_str);
        return 0; /* OK */
    default:
        ; /* ANSI C compiler needs it */
    }
    return 1; /* FAILED */
}
