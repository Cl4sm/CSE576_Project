int
httpPrintCacheControl(char *buf, int offset, int len,
                      int flags, CacheControlPtr cache_control)
{
    int n = offset;
    int sub = 0;

#define PRINT_SEP() \
    do {\
        n = cachePrintSeparator(buf, n, len, sub); \
        sub = 1; \
    } while(0)

    if(cache_control)
        flags |= cache_control->flags;

    if(flags & CACHE_NO) {
        PRINT_SEP();
        n = snnprintf(buf, n, len, "no-cache");
    }
    if(flags & CACHE_PUBLIC) {
        PRINT_SEP();
        n = snnprintf(buf, n, len, "public");
    }
    if(flags & CACHE_PRIVATE) {
        PRINT_SEP();
        n = snnprintf(buf, n, len, "private");
    }
    if(flags & CACHE_NO_STORE) {
        PRINT_SEP();
        n = snnprintf(buf, n, len, "no-store");
    }
    if(flags & CACHE_NO_TRANSFORM) {
        PRINT_SEP();
        n = snnprintf(buf, n, len, "no-transform");
    }
    if(flags & CACHE_MUST_REVALIDATE) {
        PRINT_SEP();
        n = snnprintf(buf, n, len, "must-revalidate");
    }
    if(flags & CACHE_PROXY_REVALIDATE) {
        PRINT_SEP();
        n = snnprintf(buf, n, len, "proxy-revalidate");
    }
    if(flags & CACHE_ONLY_IF_CACHED) {
        PRINT_SEP();
        n = snnprintf(buf, n, len, "only-if-cached");
    }
    if(cache_control) {
        if(cache_control->max_age >= 0) {
            PRINT_SEP();
            n = snnprintf(buf, n, len, "max-age=%d",
                          cache_control->max_age);
        }
        if(cache_control->s_maxage >= 0) {
            PRINT_SEP();
            n = snnprintf(buf, n, len, "s-maxage=%d", 
                          cache_control->s_maxage);
        }
        if(cache_control->min_fresh > 0) {
            PRINT_SEP();
            n = snnprintf(buf, n, len, "min-fresh=%d",
                          cache_control->min_fresh);
        }
        if(cache_control->max_stale > 0) {
            PRINT_SEP();
            n = snnprintf(buf, n, len, "max-stale=%d",
                          cache_control->min_fresh);
        }
    }
    return n;
#undef PRINT_SEP
}