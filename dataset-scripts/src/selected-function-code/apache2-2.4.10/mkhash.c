int mkhash(struct passwd_ctx *ctx)
{
    char *pw;
    char salt[16];
    apr_status_t rv;
    int ret = 0;
#if CRYPT_ALGO_SUPPORTED
    char *cbuf;
#endif

    if (ctx->cost != 0 && ctx->alg != ALG_BCRYPT) {
        apr_file_printf(errfile,
                        "Warning: Ignoring -C argument for this algorithm." NL);
    }

    if (ctx->passwd == NULL) {
        if ((ret = get_password(ctx)) != 0)
            return ret;
    }
    pw = ctx->passwd;

    switch (ctx->alg) {
    case ALG_APSHA:
        /* XXX out >= 28 + strlen(sha1) chars - fixed len SHA */
        apr_sha1_base64(pw, strlen(pw), ctx->out);
        break;

    case ALG_APMD5:
        ret = generate_salt(salt, 8, &ctx->errstr, ctx->pool);
        if (ret != 0)
            break;
        rv = apr_md5_encode(pw, salt, ctx->out, ctx->out_len);
        if (rv != APR_SUCCESS) {
            ctx->errstr = apr_psprintf(ctx->pool,
                                       "could not encode password: %pm", &rv);
            ret = ERR_GENERAL;
        }
        break;

    case ALG_PLAIN:
        /* XXX this len limitation is not in sync with any HTTPd len. */
        apr_cpystrn(ctx->out, pw, ctx->out_len);
        break;

#if CRYPT_ALGO_SUPPORTED
    case ALG_CRYPT:
        ret = generate_salt(salt, 8, &ctx->errstr, ctx->pool);
        if (ret != 0)
            break;
        cbuf = crypt(pw, salt);
        if (cbuf == NULL) {
            rv = APR_FROM_OS_ERROR(errno);
            ctx->errstr = apr_psprintf(ctx->pool, "crypt() failed: %pm", &rv);
            ret = ERR_PWMISMATCH;
            break;
        }

        apr_cpystrn(ctx->out, cbuf, ctx->out_len - 1);
        if (strlen(pw) > 8) {
            char *truncpw = apr_pstrdup(ctx->pool, pw);
            truncpw[8] = '\0';
            if (!strcmp(ctx->out, crypt(truncpw, salt))) {
                apr_file_printf(errfile, "Warning: Password truncated to 8 "
                                "characters by CRYPT algorithm." NL);
            }
            memset(truncpw, '\0', strlen(pw));
        }
        break;
#endif /* CRYPT_ALGO_SUPPORTED */

#if BCRYPT_ALGO_SUPPORTED
    case ALG_BCRYPT:
        rv = apr_generate_random_bytes((unsigned char*)salt, 16);
        if (rv != APR_SUCCESS) {
            ctx->errstr = apr_psprintf(ctx->pool, "Unable to generate random "
                                       "bytes: %pm", &rv);
            ret = ERR_RANDOM;
            break;
        }

        if (ctx->cost == 0)
            ctx->cost = BCRYPT_DEFAULT_COST;
        rv = apr_bcrypt_encode(pw, ctx->cost, (unsigned char*)salt, 16,
                               ctx->out, ctx->out_len);
        if (rv != APR_SUCCESS) {
            ctx->errstr = apr_psprintf(ctx->pool, "Unable to encode with "
                                       "bcrypt: %pm", &rv);
            ret = ERR_PWMISMATCH;
            break;
        }
        break;
#endif /* BCRYPT_ALGO_SUPPORTED */

    default:
        apr_file_printf(errfile, "mkhash(): BUG: invalid algorithm %d",
                        ctx->alg);
        abort();
    }
    memset(pw, '\0', strlen(pw));
    return ret;
}