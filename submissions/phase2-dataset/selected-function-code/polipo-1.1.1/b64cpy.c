int
b64cpy(char *restrict dst, const char *restrict src, int n, int fss)
{
    const char *b = fss ? b64fss: b64;
    int i, j;

    j = 0;
    for(i = 0; i < n; i += 3) {
        unsigned char a0, a1, a2;
        a0 = src[i];
        a1 = i < n - 1 ? src[i + 1] : 0;
        a2 = i < n - 2 ? src[i + 2] : 0;
        dst[j++] = b[(a0 >> 2) & 0x3F];
        dst[j++] = b[((a0 << 4) & 0x30) | ((a1 >> 4) & 0x0F)];
        if(i < n - 1)
            dst[j++] = b[((a1 << 2) & 0x3C) | ((a2 >> 6) & 0x03)];
        else
            dst[j++] = '=';
        if(i < n - 2)
            dst[j++] = b[a2 & 0x3F];
        else
            dst[j++] = '=';
    }
    return j;
}