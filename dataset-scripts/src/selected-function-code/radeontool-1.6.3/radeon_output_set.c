void radeon_output_set(char *output, char *status)
{
    int on = (strncmp(status, "en", 2) == 0);

    if (strcmp(output, "tmds1") == 0) {
        if (on) {
            SET_REG(AVIVO_TMDSA_TRANSMITTER_CONTROL, 0x10000011);
            SET_REG(AVIVO_TMDSA_TRANSMITTER_ENABLE, 0x0000001f);
            SET_REG(AVIVO_TMDSA_CNTL, 0x00001010 | AVIVO_TMDSA_CNTL_ENABLE);
        }
        else {
            SET_REG(AVIVO_TMDSA_CNTL, 0x00001010);
            SET_REG(AVIVO_TMDSA_BIT_DEPTH_CONTROL, 0x04000000);
            SET_REG(AVIVO_TMDSA_DATA_SYNCHRONIZATION, 0x00000000);
            SET_REG(AVIVO_TMDSA_TRANSMITTER_CONTROL, 0x10000011);
            SET_REG(AVIVO_TMDSA_TRANSMITTER_ENABLE, 0x00060000);
        }
    }
    else if (strcmp(output, "tmds2") == 0) {
        if (on) {
            SET_REG(AVIVO_LVTMA_TRANSMITTER_CONTROL, 0x30000011);
            SET_REG(AVIVO_LVTMA_TRANSMITTER_ENABLE, 0x0000003e);
            SET_REG(AVIVO_LVTMA_CNTL, 0x00001010 | AVIVO_TMDSA_CNTL_ENABLE);
        }
        else {
            SET_REG(AVIVO_LVTMA_CNTL, 0x1010);
            SET_REG(AVIVO_LVTMA_BIT_DEPTH_CONTROL, 0x04000000);
            SET_REG(AVIVO_LVTMA_DATA_SYNCHRONIZATION, 0x00000000);
            SET_REG(AVIVO_LVTMA_TRANSMITTER_CONTROL, 0x10000011);
            SET_REG(AVIVO_LVTMA_TRANSMITTER_ENABLE, 0x00060000);
        }
    }
    else if (strcmp(output, "dac1") == 0) {

        if (on) {
            SET_REG(AVIVO_DACA_FORCE_OUTPUT_CNTL, 0x00000000);
            SET_REG(AVIVO_DACA_POWERDOWN, 0x00000000);
            SET_REG(AVIVO_DACA_ENABLE, AVIVO_DAC_ENABLE);
        }
        else {
            SET_REG(AVIVO_DACA_ENABLE, 0x00000000);
            SET_REG(AVIVO_DACA_FORCE_OUTPUT_CNTL, (AVIVO_DACA_FORCE_OUTPUT_CNTL_FORCE_DATA_EN | ((AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_SEL_BLUE | AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_SEL_GREEN | AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_SEL_RED) << AVIVO_DACA_FORCE_OUTPUT_CNTL_DATA_SEL_SHIFT)));
            SET_REG(AVIVO_DACA_POWERDOWN, (AVIVO_DACA_POWERDOWN_POWERDOWN | AVIVO_DACA_POWERDOWN_BLUE | AVIVO_DACA_POWERDOWN_GREEN | AVIVO_DACA_POWERDOWN_RED));
        }
    }
    else if (strcmp(output, "dac2") == 0) {
        if (on) {
            SET_REG(AVIVO_DACB_FORCE_OUTPUT_CNTL, 0x00000000);
            SET_REG(AVIVO_DACB_POWERDOWN, 0x00000000);
            SET_REG(AVIVO_DACB_ENABLE, AVIVO_DAC_ENABLE);
 
	    SET_REG(AVIVO_DACB_FORCE_OUTPUT_CNTL, (AVIVO_DACB_FORCE_OUTPUT_CNTL_FORCE_DATA_EN | ((AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_SEL_BLUE | AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_SEL_GREEN | AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_SEL_RED) << AVIVO_DACB_FORCE_OUTPUT_CNTL_DATA_SEL_SHIFT)));
	    //            SET_REG(AVIVO_DACB_POWERDOWN, (AVIVO_DACB_POWERDOWN_POWERDOWN | AVIVO_DACB_POWERDOWN_BLUE | AVIVO_DACB_POWERDOWN_GREEN | AVIVO_DACB_POWERDOWN_RED));

        }
        else {
            SET_REG(AVIVO_DACB_ENABLE, 0x00000000);
        }
    }
    else if (strcmp(output, "crtc1") == 0) {
        if (on) {
            /* Select graphics mode? */
            SET_REG(0x00000330, 0x00010600);
            SET_REG(0x00000338, 0x00000400);
            SET_REG(AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS, fb_base);
            /* These modelines are all hardcoded for my 1280x1024;
             * adjust to suit. */
            SET_REG(AVIVO_D1GRPH_SECONDARY_SURFACE_ADDRESS,
                    fb_base + 1280 * 1024 * 4);

            SET_REG(AVIVO_D1GRPH_X_END, 1280);
            SET_REG(AVIVO_D1GRPH_Y_END, 1024);
            SET_REG(AVIVO_D1GRPH_PITCH, 1280);
            SET_REG(AVIVO_D1CRTC_H_TOTAL, 1687);
            SET_REG(AVIVO_D1CRTC_H_BLANK_START_END, 1672 | (392 << 16));
            SET_REG(AVIVO_D1CRTC_H_SYNC_A, (144 << 16));
            SET_REG(AVIVO_D1CRTC_H_SYNC_A_CNTL, 0x00000000);
            SET_REG(AVIVO_D1CRTC_V_TOTAL, 1065);
            SET_REG(AVIVO_D1CRTC_V_BLANK_START_END, 1065 | (41 << 16));
            SET_REG(AVIVO_D1CRTC_V_SYNC_A, (3 << 16));
            SET_REG(AVIVO_D1CRTC_V_SYNC_A_CNTL, 0x00000000);
            SET_REG(AVIVO_D1GRPH_CONTROL,  AVIVO_D1GRPH_CONTROL_DEPTH_32BPP | AVIVO_D1GRPH_CONTROL_32BPP_ARGB8888);
            SET_REG(AVIVO_D1CRTC_CONTROL, AVIVO_CRTC_EN);
            SET_REG(AVIVO_D1GRPH_ENABLE, 1);
        }
        else {
            SET_REG(AVIVO_D1GRPH_ENABLE, 0x00000000);
            SET_REG(AVIVO_D1CRTC_CONTROL, 0x00000000);
        }
    }
    else if (strcmp(output, "lvds") == 0) {
        if (on)
            SET_REG(AVIVO_LVTMA_PWRSEQ_CNTL, GET_REG(AVIVO_LVTMA_PWRSEQ_CNTL) | AVIVO_LVTMA_PWRSEQ_EN);
        else
            SET_REG(AVIVO_LVTMA_PWRSEQ_CNTL, GET_REG(AVIVO_LVTMA_PWRSEQ_CNTL) &
                                      ~(AVIVO_LVTMA_PWRSEQ_EN));
    }
    else if (strcmp(output, "cur1") == 0) {
#if 0
        if (on) {
            SET_REG(AVIVO_D1CUR_LOCATION, GET_REG(AVIVO_CRTC1_FB_END));
            SET_REG(AVIVO_D1CUR_SIZE, (32 << 16) | 32);
            SET_REG(AVIVO_D1CUR_CONTROL, AVIVO_CURSOR_EN |
                                        (AVIVO_CURSOR_FORMAT_ARGB <<
                                         AVIVO_CURSOR_FORMAT_SHIFT));
        }
        else {
            SET_REG(AVIVO_D1CUR_CONTROL, 0x00000000);
        }
#endif
    }
    else {
        die("unknown output");
    }
}