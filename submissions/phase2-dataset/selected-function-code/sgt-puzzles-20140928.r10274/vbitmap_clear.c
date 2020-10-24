static int vbitmap_clear(int w, int h, struct solver_scratch *sc,
                         int x, int y, int vbits, char *reason, ...)
{
    int done_something = FALSE;
    int vbit;

    for (vbit = 1; vbit <= 8; vbit <<= 1)
        if (vbits & sc->vbitmap[y*w+x] & vbit) {
            done_something = TRUE;
#ifdef SOLVER_DIAGNOSTICS
            if (verbose) {
                va_list ap;

                printf("ruling out %c shape at (%d,%d)-(%d,%d) (",
                       "!v^!>!!!<"[vbit], x, y,
                       x+((vbit&0x3)!=0), y+((vbit&0xC)!=0));

                va_start(ap, reason);
                vprintf(reason, ap);
                va_end(ap);

                printf(")\n");
            }
#endif
            sc->vbitmap[y*w+x] &= ~vbit;
        }

    return done_something;
}