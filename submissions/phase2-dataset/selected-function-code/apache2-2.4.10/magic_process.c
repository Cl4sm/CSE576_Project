static int magic_process(request_rec *r)
{
    apr_file_t *fd = NULL;
    unsigned char buf[HOWMANY + 1];  /* one extra for terminating '\0' */
    apr_size_t nbytes = 0;           /* number of bytes read from a datafile */
    int result;

    /*
     * first try judging the file based on its filesystem status
     */
    switch ((result = fsmagic(r, r->filename))) {
    case DONE:
        magic_rsl_putchar(r, '\n');
        return OK;
    case OK:
        break;
    default:
        /* fatal error, bail out */
        return result;
    }

    if (apr_file_open(&fd, r->filename, APR_READ, APR_OS_DEFAULT, r->pool) != APR_SUCCESS) {
        /* We can't open it, but we were able to stat it. */
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01512)
                    MODNAME ": can't read `%s'", r->filename);
        /* let some other handler decide what the problem is */
        return DECLINED;
    }

    /*
     * try looking at the first HOWMANY bytes
     */
    nbytes = sizeof(buf) - 1;
    if ((result = apr_file_read(fd, (char *) buf, &nbytes)) != APR_SUCCESS) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, result, r, APLOGNO(01513)
                    MODNAME ": read failed: %s", r->filename);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    if (nbytes == 0) {
        return DECLINED;
    }
    else {
        buf[nbytes++] = '\0';  /* null-terminate it */
        result = tryit(r, buf, nbytes, 1);
        if (result != OK) {
            return result;
        }
    }

    (void) apr_file_close(fd);
    (void) magic_rsl_putchar(r, '\n');

    return OK;
}