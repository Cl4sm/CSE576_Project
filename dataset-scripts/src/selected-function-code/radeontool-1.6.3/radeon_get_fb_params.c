int radeon_get_fb_params(char *crtc, int write, unsigned long *location, int *len)
{
    unsigned long format;
#if 0
    if (strcmp(crtc, "crtc1") == 0 || strcmp(crtc, "1") == 0) {
        *location = GET_REG(AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS);
        *len = GET_REG(AVIVO_CRTC1_FB_END) - *location;
    }
    else if (strcmp(crtc, "crtc2") == 0 || strcmp(crtc, "2") == 0) {
        *location = GET_REG(AVIVO_D2CRTC_FB_LOCATION);
        *len = GET_REG(AVIVO_D2CRTC_FB_END) - *location;
    }
    else if (strcmp(crtc, "cur1") == 0) {
        *location = GET_REG(AVIVO_D1CUR_LOCATION);
        *len = 64 * 64;
        format = (GET_REG(AVIVO_D1CUR_CONTROL) & AVIVO_CURSOR_FORMAT_MASK) >>
                 AVIVO_CURSOR_FORMAT_SHIFT;

        if (format == AVIVO_CURSOR_FORMAT_ARGB ||
            format == AVIVO_CURSOR_FORMAT_ABGR) {
            *len *= 4;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }

    *location -= fb_base;
    *location += (unsigned long) fb_mem;
#endif
    return 1;
}