void
redirectorTrigger(void)
{
    RedirectRequestPtr request = redirector_request_first;
    int rc;

    if(!request)
        return;

    if(redirector_read_fd < 0) {
        rc = runRedirector(&redirector_pid,
                           &redirector_read_fd, &redirector_write_fd);
        if(rc < 0) {
            request->handler(rc, request->url, NULL, NULL, request->data);
            redirectorDestroyRequest(request);
            return;
        }
    }
    do_stream_2(IO_WRITE, redirector_write_fd, 0,
                request->url->string, request->url->length,
                "\n", 1,
                redirectorStreamHandler1, request);
}