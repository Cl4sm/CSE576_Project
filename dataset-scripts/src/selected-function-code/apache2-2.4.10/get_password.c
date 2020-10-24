int get_password(struct passwd_ctx *ctx)
{
    char buf[MAX_STRING_LEN + 1];
    if (ctx->passwd_src == PW_STDIN) {
        apr_file_t *file_stdin;
        apr_size_t nread;
        if (apr_file_open_stdin(&file_stdin, ctx->pool) != APR_SUCCESS) {
            ctx->errstr = "Unable to read from stdin.";
            return ERR_GENERAL;
        }
        if (apr_file_read_full(file_stdin, buf, sizeof(buf) - 1,
                               &nread) != APR_EOF
            || nread == sizeof(buf) - 1) {
            goto err_too_long;
        }
        buf[nread] = '\0';
        if (nread >= 1 && buf[nread-1] == '\n') {
            buf[nread-1] = '\0';
            if (nread >= 2 && buf[nread-2] == '\r')
                buf[nread-2] = '\0';
        }
        apr_file_close(file_stdin);
        ctx->passwd = apr_pstrdup(ctx->pool, buf);
    }
    else if (ctx->passwd_src == PW_PROMPT_VERIFY) {
        apr_size_t bufsize = sizeof(buf);
        if (apr_password_get("Enter password: ", buf, &bufsize) != 0)
            goto err_too_long;
        ctx->passwd = apr_pstrdup(ctx->pool, buf);
    }
    else {
        apr_size_t bufsize = sizeof(buf);
        if (apr_password_get("New password: ", buf, &bufsize) != 0)
            goto err_too_long;
        ctx->passwd = apr_pstrdup(ctx->pool, buf);
        bufsize = sizeof(buf);
        buf[0] = '\0';
        apr_password_get("Re-type new password: ", buf, &bufsize);
        if (strcmp(ctx->passwd, buf) != 0) {
            ctx->errstr = "password verification error";
            memset(ctx->passwd, '\0', strlen(ctx->passwd));
            memset(buf, '\0', sizeof(buf));
            return ERR_PWMISMATCH;
        }
    }
    memset(buf, '\0', sizeof(buf));
    return 0;

err_too_long:
    ctx->errstr = apr_psprintf(ctx->pool,
                               "password too long (>%" APR_SIZE_T_FMT ")",
                               ctx->out_len - 1);
    return ERR_OVERFLOW;
}