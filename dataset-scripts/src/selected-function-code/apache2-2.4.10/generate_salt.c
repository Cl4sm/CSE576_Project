static int generate_salt(char *s, size_t size, const char **errstr,
                         apr_pool_t *pool)
{
    unsigned char rnd[32];
    static const char itoa64[] =
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    apr_size_t n;
    unsigned int val = 0, bits = 0;
    apr_status_t rv;

    n = (size * 6 + 7)/8;
    if (n > sizeof(rnd)) {
        apr_file_printf(errfile, "generate_salt(): BUG: Buffer too small");
        abort();
    }
    rv = apr_generate_random_bytes(rnd, n);
    if (rv) {
        *errstr = apr_psprintf(pool, "Unable to generate random bytes: %pm",
                               &rv);
        return ERR_RANDOM;
    }
    n = 0;
    while (size > 0) {
        if (bits < 6) {
            val |= (rnd[n++] << bits);
            bits += 8;
        }
        *s++ = itoa64[val & 0x3f];
        size--;
        val >>= 6;
        bits -= 6;
   }
   *s = '\0';
   return 0;
}