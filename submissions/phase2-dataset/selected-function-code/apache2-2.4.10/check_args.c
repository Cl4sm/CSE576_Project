static void check_args(int argc, const char *const argv[],
                       struct passwd_ctx *ctx, unsigned *mask, char **user,
                       char **pwfilename)
{
    const char *arg;
    int args_left = 2;
    int i, ret;
    apr_getopt_t *state;
    apr_status_t rv;
    char opt;
    const char *opt_arg;
    apr_pool_t *pool = ctx->pool;

    rv = apr_getopt_init(&state, pool, argc, argv);
    if (rv != APR_SUCCESS)
        exit(ERR_SYNTAX);

    while ((rv = apr_getopt(state, "cnmspdBbDiC:v", &opt, &opt_arg)) == APR_SUCCESS) {
        switch (opt) {
        case 'c':
            *mask |= APHTP_NEWFILE;
            break;
        case 'n':
            args_left--;
            *mask |= APHTP_NOFILE;
            break;
        case 'D':
            *mask |= APHTP_DELUSER;
            break;
        case 'v':
            *mask |= APHTP_VERIFY;
            break;
        default:
            ret = parse_common_options(ctx, opt, opt_arg);
            if (ret) {
                apr_file_printf(errfile, "%s: %s" NL, argv[0], ctx->errstr);
                exit(ret);
            }
        }
    }
    if (ctx->passwd_src == PW_ARG)
        args_left++;
    if (rv != APR_EOF)
        usage();

    if ((*mask) & (*mask - 1)) {
        /* not a power of two, i.e. more than one flag specified */
        apr_file_printf(errfile, "%s: only one of -c -n -v -D may be specified" NL,
            argv[0]);
        exit(ERR_SYNTAX);
    }
    if ((*mask & APHTP_VERIFY) && ctx->passwd_src == PW_PROMPT)
        ctx->passwd_src = PW_PROMPT_VERIFY;

    /*
     * Make sure we still have exactly the right number of arguments left
     * (the filename, the username, and possibly the password if -b was
     * specified).
     */
    i = state->ind;
    if ((argc - i) != args_left) {
        usage();
    }

    if (!(*mask & APHTP_NOFILE)) {
        if (strlen(argv[i]) > (APR_PATH_MAX - 1)) {
            apr_file_printf(errfile, "%s: filename too long" NL, argv[0]);
            exit(ERR_OVERFLOW);
        }
        *pwfilename = apr_pstrdup(pool, argv[i++]);
    }
    if (strlen(argv[i]) > (MAX_STRING_LEN - 1)) {
        apr_file_printf(errfile, "%s: username too long (> %d)" NL,
                        argv[0], MAX_STRING_LEN - 1);
        exit(ERR_OVERFLOW);
    }
    *user = apr_pstrdup(pool, argv[i++]);
    if ((arg = strchr(*user, ':')) != NULL) {
        apr_file_printf(errfile, "%s: username contains illegal "
                        "character '%c'" NL, argv[0], *arg);
        exit(ERR_BADUSER);
    }
    if (ctx->passwd_src == PW_ARG) {
        if (strlen(argv[i]) > (MAX_STRING_LEN - 1)) {
            apr_file_printf(errfile, "%s: password too long (> %d)" NL,
                argv[0], MAX_STRING_LEN);
            exit(ERR_OVERFLOW);
        }
        ctx->passwd = apr_pstrdup(pool, argv[i]);
    }
}