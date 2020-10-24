static apr_status_t delete_url(apr_pool_t *pool, const char *proxypath, const char *url)
{
    apr_md5_ctx_t context;
    unsigned char digest[16];
    char tmp[23];
    int i, k;
    unsigned int x;
    static const char enc_table[64] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_@";

    apr_md5_init(&context);
    apr_md5_update(&context, (const unsigned char *) url, strlen(url));
    apr_md5_final(digest, &context);

    /* encode 128 bits as 22 characters, using a modified uuencoding
     * the encoding is 3 bytes -> 4 characters* i.e. 128 bits is
     * 5 x 3 bytes + 1 byte -> 5 * 4 characters + 2 characters
     */
    for (i = 0, k = 0; i < 15; i += 3) {
        x = (digest[i] << 16) | (digest[i + 1] << 8) | digest[i + 2];
        tmp[k++] = enc_table[x >> 18];
        tmp[k++] = enc_table[(x >> 12) & 0x3f];
        tmp[k++] = enc_table[(x >> 6) & 0x3f];
        tmp[k++] = enc_table[x & 0x3f];
    }

    /* one byte left */
    x = digest[15];
    tmp[k++] = enc_table[x >> 2]; /* use up 6 bits */
    tmp[k++] = enc_table[(x << 4) & 0x3f];
    tmp[k] = 0;

    /* automatically find the directory levels */
    return find_directory(pool, proxypath, tmp);
}