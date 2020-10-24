static int create_uncompress_child(struct uncompress_parms *parm, apr_pool_t *cntxt,
                                   apr_file_t **pipe_in)
{
    int rc = 1;
    const char *new_argv[4];
    request_rec *r = parm->r;
    apr_pool_t *child_context = cntxt;
    apr_procattr_t *procattr;
    apr_proc_t *procnew;

    /* XXX missing 1.3 logic:
     *
     * what happens when !compr[parm->method].silent?
     * Should we create the err pipe, read it, and copy to the log?
     */

    if ((apr_procattr_create(&procattr, child_context) != APR_SUCCESS) ||
        (apr_procattr_io_set(procattr, APR_FULL_BLOCK,
                           APR_FULL_BLOCK, APR_NO_PIPE)   != APR_SUCCESS) ||
        (apr_procattr_dir_set(procattr,
                              ap_make_dirstr_parent(r->pool, r->filename)) != APR_SUCCESS) ||
        (apr_procattr_cmdtype_set(procattr, APR_PROGRAM_PATH) != APR_SUCCESS)) {
        /* Something bad happened, tell the world. */
        ap_log_rerror(APLOG_MARK, APLOG_ERR, APR_ENOPROC, r, APLOGNO(01551)
               "couldn't setup child process: %s", r->filename);
    }
    else {
        new_argv[0] = compr[parm->method].argv[0];
        new_argv[1] = compr[parm->method].argv[1];
        new_argv[2] = r->filename;
        new_argv[3] = NULL;

        procnew = apr_pcalloc(child_context, sizeof(*procnew));
        rc = apr_proc_create(procnew, compr[parm->method].argv[0],
                               new_argv, NULL, procattr, child_context);

        if (rc != APR_SUCCESS) {
            /* Bad things happened. Everyone should have cleaned up. */
            ap_log_rerror(APLOG_MARK, APLOG_ERR, APR_ENOPROC, r, APLOGNO(01552)
                          MODNAME ": could not execute `%s'.",
                          compr[parm->method].argv[0]);
        }
        else {
            apr_pool_note_subprocess(child_context, procnew, APR_KILL_AFTER_TIMEOUT);
            *pipe_in = procnew->out;
        }
    }

    return (rc);
}