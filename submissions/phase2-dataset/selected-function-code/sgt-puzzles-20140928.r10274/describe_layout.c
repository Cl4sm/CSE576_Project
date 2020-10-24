static char *describe_layout(char *grid, int area, int x, int y,
                             int obfuscate)
{
    char *ret, *p;
    unsigned char *bmp;
    int i;

    /*
     * Set up the mine bitmap and obfuscate it.
     */
    bmp = snewn((area + 7) / 8, unsigned char);
    memset(bmp, 0, (area + 7) / 8);
    for (i = 0; i < area; i++) {
        if (grid[i])
            bmp[i / 8] |= 0x80 >> (i % 8);
    }
    if (obfuscate)
        obfuscate_bitmap(bmp, area, FALSE);

    /*
     * Now encode the resulting bitmap in hex. We can work to
     * nibble rather than byte granularity, since the obfuscation
     * function guarantees to return a bit string of the same
     * length as its input.
     */
    ret = snewn((area+3)/4 + 100, char);
    p = ret + sprintf(ret, "%d,%d,%s", x, y,
                      obfuscate ? "m" : "u");   /* 'm' == masked */
    for (i = 0; i < (area+3)/4; i++) {
        int v = bmp[i/2];
        if (i % 2 == 0)
            v >>= 4;
        *p++ = "0123456789abcdef"[v & 0xF];
    }
    *p = '\0';

    sfree(bmp);

    return ret;
}