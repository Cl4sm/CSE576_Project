int parse_common_options(struct passwd_ctx *ctx, char opt,
                          const char *opt_arg)
{
    switch (opt) {
    case 'b':
        ctx->passwd_src = PW_ARG;
        break;
    case 'i':
        ctx->passwd_src = PW_STDIN;
        break;
    case 'm':
        ctx->alg = ALG_APMD5;
        break;
    case 's':
        ctx->alg = ALG_APSHA;
        break;
    case 'p':
        ctx->alg = ALG_PLAIN;
#if !PLAIN_ALGO_SUPPORTED
        /* Backward compatible behavior: Just print a warning */
        apr_file_printf(errfile,
                        "Warning: storing passwords as plain text might just "
                        "not work on this platform." NL);
#endif
        break;
    case 'd':
#if CRYPT_ALGO_SUPPORTED
        ctx->alg = ALG_CRYPT;
#else
        /* Backward compatible behavior: Use MD5. OK since MD5 is more secure */
        apr_file_printf(errfile,
                        "Warning: CRYPT algorithm not supported on this "
                        "platform." NL
                        "Automatically using MD5 format." NL);
        ctx->alg = ALG_APMD5;
#endif
        break;
    case 'B':
#if BCRYPT_ALGO_SUPPORTED
        ctx->alg = ALG_BCRYPT;
#else
        /* Don't fall back to something less secure */
        ctx->errstr = "BCRYPT algorithm not supported on this platform";
        return ERR_ALG_NOT_SUPP;
#endif
        break;
    case 'C': {
            char *endptr;
            long num = strtol(opt_arg, &endptr, 10);
            if (*endptr != '\0' || num <= 0) {
                ctx->errstr = "argument to -C must be a positive integer";
                return ERR_SYNTAX;
            }
            ctx->cost = num;
            break;
        }
    default:
        apr_file_printf(errfile, 
                        "parse_common_options(): BUG: invalid option %c",
                        opt);
        abort();
    }
    return 0;
}