static int unique_id_global_init(apr_pool_t *p, apr_pool_t *plog, apr_pool_t *ptemp, server_rec *main_server)
{
    char str[APRMAXHOSTLEN + 1];
    apr_status_t rv;
    char *ipaddrstr;
    apr_sockaddr_t *sockaddr;

    /*
     * Calculate the sizes and offsets in cur_unique_id.
     */
    unique_id_rec_offset[0] = APR_OFFSETOF(unique_id_rec, stamp);
    unique_id_rec_size[0] = sizeof(cur_unique_id.stamp);
    unique_id_rec_offset[1] = APR_OFFSETOF(unique_id_rec, in_addr);
    unique_id_rec_size[1] = sizeof(cur_unique_id.in_addr);
    unique_id_rec_offset[2] = APR_OFFSETOF(unique_id_rec, pid);
    unique_id_rec_size[2] = sizeof(cur_unique_id.pid);
    unique_id_rec_offset[3] = APR_OFFSETOF(unique_id_rec, counter);
    unique_id_rec_size[3] = sizeof(cur_unique_id.counter);
    unique_id_rec_offset[4] = APR_OFFSETOF(unique_id_rec, thread_index);
    unique_id_rec_size[4] = sizeof(cur_unique_id.thread_index);
    unique_id_rec_total_size = unique_id_rec_size[0] + unique_id_rec_size[1] +
                               unique_id_rec_size[2] + unique_id_rec_size[3] +
                               unique_id_rec_size[4];

    /*
     * Calculate the size of the structure when encoded.
     */
    unique_id_rec_size_uu = (unique_id_rec_total_size*8+5)/6;

    /*
     * Now get the global in_addr.  Note that it is not sufficient to use one
     * of the addresses from the main_server, since those aren't as likely to
     * be unique as the physical address of the machine
     */
    if ((rv = apr_gethostname(str, sizeof(str) - 1, p)) != APR_SUCCESS) {
        ap_log_error(APLOG_MARK, APLOG_ALERT, rv, main_server, APLOGNO(01563)
          "unable to find hostname of the server");
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    if ((rv = apr_sockaddr_info_get(&sockaddr, str, AF_INET, 0, 0, p)) == APR_SUCCESS) {
        global_in_addr = sockaddr->sa.sin.sin_addr.s_addr;
    }
    else {
        ap_log_error(APLOG_MARK, APLOG_ALERT, rv, main_server, APLOGNO(01564)
                    "unable to find IPv4 address of \"%s\"", str);
#if APR_HAVE_IPV6
        if ((rv = apr_sockaddr_info_get(&sockaddr, str, AF_INET6, 0, 0, p)) == APR_SUCCESS) {
            memcpy(&global_in_addr,
                   (char *)sockaddr->ipaddr_ptr + sockaddr->ipaddr_len - sizeof(global_in_addr),
                   sizeof(global_in_addr));
            ap_log_error(APLOG_MARK, APLOG_ALERT, rv, main_server, APLOGNO(01565)
                         "using low-order bits of IPv6 address "
                         "as if they were unique");
        }
        else
#endif
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    apr_sockaddr_ip_get(&ipaddrstr, sockaddr);
    ap_log_error(APLOG_MARK, APLOG_INFO, 0, main_server, APLOGNO(01566) "using ip addr %s",
                 ipaddrstr);

    /*
     * If the server is pummelled with restart requests we could possibly end
     * up in a situation where we're starting again during the same second
     * that has been used in previous identifiers.  Avoid that situation.
     *
     * In truth, for this to actually happen not only would it have to restart
     * in the same second, but it would have to somehow get the same pids as
     * one of the other servers that was running in that second. Which would
     * mean a 64k wraparound on pids ... not very likely at all.
     *
     * But protecting against it is relatively cheap.  We just sleep into the
     * next second.
     */
    apr_sleep(apr_time_from_sec(1) - apr_time_usec(apr_time_now()));
    return OK;
}