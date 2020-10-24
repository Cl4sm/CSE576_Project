unsigned char *hex2bin(const char *in, int outlen)
{
    unsigned char *ret = snewn(outlen, unsigned char);
    int i;

    memset(ret, 0, outlen*sizeof(unsigned char));
    for (i = 0; i < outlen*2; i++) {
        int c = in[i];
        int v;

        assert(c != 0);
        if (c >= '0' && c <= '9')
            v = c - '0';
        else if (c >= 'a' && c <= 'f')
            v = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            v = c - 'A' + 10;
        else
            v = 0;

        ret[i / 2] |= v << (4 * (1 - (i % 2)));
    }
    return ret;
}