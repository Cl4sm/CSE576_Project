static void set_polled_events(struct connection *c, apr_int16_t new_reqevents)
{
    apr_status_t rv;

    if (c->pollfd.reqevents != new_reqevents) {
        if (c->pollfd.reqevents != 0) {
            rv = apr_pollset_remove(readbits, &c->pollfd);
            if (rv != APR_SUCCESS) {
                apr_err("apr_pollset_remove()", rv);
            }
        }

        if (new_reqevents != 0) {
            c->pollfd.reqevents = new_reqevents;
            rv = apr_pollset_add(readbits, &c->pollfd);
            if (rv != APR_SUCCESS) {
                apr_err("apr_pollset_add()", rv);
            }
        }
    }
}