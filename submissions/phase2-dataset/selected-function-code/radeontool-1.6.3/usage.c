static void usage(void)
{
    const char *dac_status, *light_status;

    if (!radeon_cntl_mem) {
        dac_status = "";
        light_status = "";
    } else {
        dac_status = (radeon_get(RADEON_DAC_CNTL, "RADEON_DAC_CNTL") & RADEON_DAC_PDWN)
            ? " (off)" : " (on)";
        light_status = (radeon_get(RADEON_LVDS_GEN_CNTL,"RADEON_LVDS_GEN_CNTL") & RADEON_LVDS_ON)
            ? " (on)" : " (off)";
    }

    fprintf(stderr,"usage: radeontool [options] [command]\n");
    fprintf(stderr,"         --debug            - show a little debug info\n");
    fprintf(stderr,"         --skip=1           - use the second radeon card\n");
    fprintf(stderr,"         dac [on|off]       - power down the external video outputs%s\n",
	   dac_status);
    fprintf(stderr,"         light [on|off]     - power down the backlight%s\n",
	   light_status);
    fprintf(stderr,"         stretch [on|off|vert|horiz|auto|manual]   - stretching for resolution mismatch \n");
    fprintf(stderr,"         regs               - show a listing of some random registers\n");
    fprintf(stderr,"         regmatch <pattern> - show registers matching wildcard pattern\n");
    fprintf(stderr,"         regset <pattern> <value> - set registers matching wildcard pattern\n");
    exit(-1);
}