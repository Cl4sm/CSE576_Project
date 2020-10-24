static int get_line(char *s, int n, apr_file_t *f)
{
    int i = 0;
    char ch;
    apr_status_t rv = APR_EINVAL;

    /* we need 2 remaining bytes in buffer */
    while (i < (n - 2) &&
           ((rv = apr_file_getc(&ch, f)) == APR_SUCCESS) && (ch != '\n')) {
        s[i++] = ch;
    }
    /* First remaining byte potentially used here */
    if (ch == '\n')
        s[i++] = ch;
    /* Second remaining byte used here */
    s[i] = '\0';

    if (rv != APR_SUCCESS)
        return 1;

    return 0;
}