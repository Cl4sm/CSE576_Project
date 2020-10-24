static void post_rotate(apr_pool_t *pool, struct logfile *newlog,
                        rotate_config_t *config, rotate_status_t *status)
{
    apr_status_t rv;
    char error[120];
    apr_procattr_t *pattr;
    const char *argv[4];
    apr_proc_t proc;

    /* Handle link file, if configured. */
    if (config->linkfile) {
        apr_file_remove(config->linkfile, newlog->pool);
        if (config->verbose) {
            fprintf(stderr,"Linking %s to %s\n", newlog->name, config->linkfile);
        }
        rv = apr_file_link(newlog->name, config->linkfile);
        if (rv != APR_SUCCESS) {
            apr_strerror(rv, error, sizeof error);
            fprintf(stderr, "Error linking file %s to %s (%s)\n",
                    newlog->name, config->linkfile, error);
            exit(2);
        }
    }

    if (!config->postrotate_prog) {
        /* Nothing more to do. */
        return;
    }

    /* Collect any zombies from a previous run, but don't wait. */
    while (apr_proc_wait_all_procs(&proc, NULL, NULL, APR_NOWAIT, pool) == APR_CHILD_DONE)
        /* noop */;

    if ((rv = apr_procattr_create(&pattr, pool)) != APR_SUCCESS) {
        fprintf(stderr,
                "post_rotate: apr_procattr_create failed for '%s': %s\n",
                config->postrotate_prog,
                apr_strerror(rv, error, sizeof(error)));
        return;
    }

    rv = apr_procattr_error_check_set(pattr, 1);
    if (rv == APR_SUCCESS)
        rv = apr_procattr_cmdtype_set(pattr, APR_PROGRAM_ENV);

    if (rv != APR_SUCCESS) {
        fprintf(stderr,
                "post_rotate: could not set up process attributes for '%s': %s\n",
                config->postrotate_prog,
                apr_strerror(rv, error, sizeof(error)));
        return;
    }

    argv[0] = config->postrotate_prog;
    argv[1] = newlog->name;
    if (status->current.fd) {
        argv[2] = status->current.name;
        argv[3] = NULL;
    }
    else {
        argv[2] = NULL;
    }

    if (config->verbose)
        fprintf(stderr, "Calling post-rotate program: %s\n", argv[0]);

    rv = apr_proc_create(&proc, argv[0], argv, NULL, pattr, pool);
    if (rv != APR_SUCCESS) {
        fprintf(stderr, "Could not spawn post-rotate process '%s': %s\n",
                config->postrotate_prog,
                apr_strerror(rv, error, sizeof(error)));
        return;
    }
}