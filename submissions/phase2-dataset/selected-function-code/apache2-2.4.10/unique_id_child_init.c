static void unique_id_child_init(apr_pool_t *p, server_rec *s)
{
    pid_t pid;

    /*
     * Note that we use the pid because it's possible that on the same
     * physical machine there are multiple servers (i.e. using Listen). But
     * it's guaranteed that none of them will share the same pids between
     * children.
     *
     * XXX: for multithread this needs to use a pid/tid combo and probably
     * needs to be expanded to 32 bits
     */
    pid = getpid();
    cur_unique_id.pid = pid;

    /*
     * Test our assumption that the pid is 32-bits.  It's possible that
     * 64-bit machines will declare pid_t to be 64 bits but only use 32
     * of them.  It would have been really nice to test this during
     * global_init ... but oh well.
     */
    if ((pid_t)cur_unique_id.pid != pid) {
        ap_log_error(APLOG_MARK, APLOG_CRIT, 0, s, APLOGNO(01567)
                    "oh no! pids are greater than 32-bits!  I'm broken!");
    }

    cur_unique_id.in_addr = global_in_addr;

    /*
     * If we use 0 as the initial counter we have a little less protection
     * against restart problems, and a little less protection against a clock
     * going backwards in time.
     */
    ap_random_insecure_bytes(&cur_unique_id.counter,
                             sizeof(cur_unique_id.counter));

    /*
     * We must always use network ordering for these bytes, so that
     * identifiers are comparable between machines of different byte
     * orderings.  Note in_addr is already in network order.
     */
    cur_unique_id.pid = htonl(cur_unique_id.pid);
}