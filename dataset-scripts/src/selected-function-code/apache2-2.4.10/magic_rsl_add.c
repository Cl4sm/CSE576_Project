static int magic_rsl_add(request_rec *r, const char *str)
{
    magic_req_rec *req_dat = (magic_req_rec *)
                    ap_get_module_config(r->request_config, &mime_magic_module);
    magic_rsl *rsl;

    /* make sure we have a list to put it in */
    if (!req_dat) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, APR_EINVAL, r, APLOGNO(01507)
                    MODNAME ": request config should not be NULL");
        if (!(req_dat = magic_set_config(r))) {
            /* failure */
            return -1;
        }
    }

    /* allocate the list entry */
    rsl = (magic_rsl *) apr_palloc(r->pool, sizeof(magic_rsl));

    /* fill it */
    rsl->str = str;
    rsl->next = (magic_rsl *) NULL;

    /* append to the list */
    if (req_dat->head && req_dat->tail) {
        req_dat->tail->next = rsl;
        req_dat->tail = rsl;
    }
    else {
        req_dat->head = req_dat->tail = rsl;
    }

    /* success */
    return 0;
}