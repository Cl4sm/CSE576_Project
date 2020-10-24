int
redirectorStreamHandler1(int status,
                         FdEventHandlerPtr event,
                         StreamRequestPtr srequest)
{
    RedirectRequestPtr request = (RedirectRequestPtr)srequest->data;

    if(status) {
        if(status >= 0)
            status = -EPIPE;
        do_log_error(L_ERROR, -status, "Write to redirector failed");
        goto fail;
    }

    if(!streamRequestDone(srequest))
        return 0;

    do_stream(IO_READ, redirector_read_fd, 0,
              redirector_buffer, REDIRECTOR_BUFFER_SIZE,
              redirectorStreamHandler2, request);
    return 1;

 fail:
    request->handler(status < 0 ? status : -EPIPE,
                     request->url, NULL, NULL, request->data);
    redirectorDestroyRequest(request);
    redirectorKill();
    return 1;
}