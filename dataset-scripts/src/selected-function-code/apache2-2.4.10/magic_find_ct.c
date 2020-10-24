static int magic_find_ct(request_rec *r)
{
    int result;
    magic_server_config_rec *conf;

    /* the file has to exist */
    if (r->finfo.filetype == APR_NOFILE || !r->filename) {
        return DECLINED;
    }

    /* was someone else already here? */
    if (r->content_type) {
        return DECLINED;
    }

    conf = ap_get_module_config(r->server->module_config, &mime_magic_module);
    if (!conf || !conf->magic) {
        return DECLINED;
    }

    /* initialize per-request info */
    if (!magic_set_config(r)) {
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    /* try excluding file-revision suffixes */
    if (revision_suffix(r) != 1) {
        /* process it based on the file contents */
        if ((result = magic_process(r)) != OK) {
            return result;
        }
    }

    /* if we have any results, put them in the request structure */
    return magic_rsl_to_request(r);
}