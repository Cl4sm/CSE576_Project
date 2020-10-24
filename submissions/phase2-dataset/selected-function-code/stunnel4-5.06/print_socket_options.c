NOEXPORT int print_socket_options(void) {
    int fd;
    socklen_t optlen;
    SOCK_OPT *ptr;
    OPT_UNION val;
    char *ta, *tl, *tr, *td;

    fd=socket(AF_INET, SOCK_STREAM, 0);

    s_log(LOG_NOTICE, " ");
    s_log(LOG_NOTICE, "Socket option defaults:");
    s_log(LOG_NOTICE,
        "    Option Name     |  Accept  |   Local  |  Remote  |OS default");
    s_log(LOG_NOTICE,
        "    ----------------+----------+----------+----------+----------");
    for(ptr=sock_opts; ptr->opt_str; ++ptr) {
        /* get OS default value */
        optlen=sizeof val;
        if(getsockopt(fd, ptr->opt_level,
                ptr->opt_name, (void *)&val, &optlen)) {
            if(get_last_socket_error()!=S_ENOPROTOOPT) {
                s_log(LOG_ERR, "Failed to get %s OS default", ptr->opt_str);
                sockerror("getsockopt");
                closesocket(fd);
                return 1; /* FAILED */
            }
            td=str_dup("write-only");
        } else
            td=print_option(ptr->opt_type, &val);
        /* get stunnel default values */
        ta=print_option(ptr->opt_type, ptr->opt_val[0]);
        tl=print_option(ptr->opt_type, ptr->opt_val[1]);
        tr=print_option(ptr->opt_type, ptr->opt_val[2]);
        /* print collected data and fee the memory */
        s_log(LOG_NOTICE, "    %-16s|%10s|%10s|%10s|%10s",
            ptr->opt_str, ta, tl, tr, td);
        str_free(ta); str_free(tl); str_free(tr); str_free(td);
    }
    closesocket(fd);
    return 0; /* OK */
}
