void eg_cmd_regs(const char *type)
{
    int show_all = (strcmp(type, "all") == 0);
    int show_mc = (show_all || strstr(type, "mc"));
    int show_hdmi = (show_all || strstr(type, "hdmi"));
    int show_grphs = 0;
    int i, j;
    uint32_t tmp, tmp1;
    char tmpname[10];
    
    if (show_mc) {
        printf("\nMemory controller:\n");
        SHOW_REG(MC_VM_FB_LOCATION);
        SHOW_REG(MC_FUS_VM_FB_OFFSET);
        SHOW_REG(MC_VM_AGP_TOP);
        SHOW_REG(MC_VM_AGP_BOT);
        SHOW_REG(MC_VM_AGP_BASE);
        SHOW_REG(MC_VM_SYSTEM_APERTURE_LOW_ADDR);
        SHOW_REG(MC_VM_SYSTEM_APERTURE_HIGH_ADDR);
        SHOW_REG(MC_VM_SYSTEM_APERTURE_DEFAULT_ADDR);
	SHOW_REG(VM_CONTEXT0_PAGE_TABLE_BASE_ADDR);
	SHOW_REG(VM_CONTEXT0_PAGE_TABLE_START_ADDR);
	SHOW_REG(VM_CONTEXT0_PAGE_TABLE_END_ADDR);
    }
    for (i = 0; i < EG_NUM_OFFSETS; i++) {
	tmp = EG_GET_MODE_REG(i, EVERGREEN_CRTC_CONTROL);
	tmp1 = EG_GET_MODE_REG(i, EVERGREEN_CUR_CONTROL);

	if (tmp & 0x1)
	    show_grphs |= (1 << i);

	printf("D%dCRTC: %s, Cursor %s\n", i+1, tmp & 0x1 ? "Enabled" : "Disabled",
	       tmp1 & 0x1 ? "Enabled" : "Disabled");
	if (!(tmp & 0x1))
	    continue;

	tmp = EG_GET_MODE_REG(i, EVERGREEN_VIEWPORT_START);
	tmp1 = EG_GET_MODE_REG(i, EVERGREEN_VIEWPORT_SIZE);
	printf("Viewport start x: %d y: %d, w %d h %d\n", tmp >> 16, tmp & 0xffff,
	       tmp1 >> 16, tmp1 & 0xffff);
	    
    }

    for (i = 0; i < EG_NUM_OFFSETS; i++) {

	if (!(show_grphs & (1 << i)))
	    continue;

	snprintf(tmpname, 10, "D%dGRPH", i+1);
	tmp = EG_GET_MODE_REG(i, EVERGREEN_GRPH_ENABLE);
	if (!(tmp & 0x1)) {
	    printf("\n%s: disabled\n", tmpname, i);
	    continue;
	}
	printf("\n%s: enabled %08x, control %08x\n", tmpname, tmp,
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_CONTROL));
	radeon_show_reg_bits(tmpname, "CONTROL", 0, 0,
			     EVERGREEN_GRPH_CONTROL + eg_offsets[i],
			     0, 1, "Depth",
			     8, 10, "Format", 
			     0, 0, NULL);
	printf("pitch %08x\n",
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_PITCH));
	printf("Primary surface address %08x:%08x\n",
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS_HIGH),
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS));
	printf("Secondary surface address %08x:%08x\n",
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS_HIGH),
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS));
	printf("Surface offset X %08x Y %08x\n",
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_SURFACE_OFFSET_X),
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_SURFACE_OFFSET_Y));
	printf("X/Y start %08x %08x, end %08x %08x\n",
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_X_START),
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_Y_START),
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_X_END),
	       EG_GET_MODE_REG(i, EVERGREEN_GRPH_Y_END));
    }

    printf("\n");
    for (i = 0; i < EG_NUM_OFFSETS; i++) {
	tmp = EG_GET_MODE_REG(i, EVERGREEN_CUR_CONTROL);
	if (!(tmp & 0x1))
	    continue;

	snprintf(tmpname, 10, "D%dCUR", i+1);
	radeon_show_reg_bits(tmpname, "CONTROL", 0, 0,
			     EVERGREEN_CUR_CONTROL,
			     0, 0, "Enable",
			     8, 10, "Mode",
			     0, 0, NULL);
	printf("Cursor surface address %08x:%08x\n",
	       EG_GET_MODE_REG(i, EVERGREEN_CUR_SURFACE_ADDRESS_HIGH),
	       EG_GET_MODE_REG(i, EVERGREEN_CUR_SURFACE_ADDRESS));
    }

    tmp = GET_REG(EVERGREEN_DACA_BASE);
    printf("\nDACA: %s\n", tmp & 0x1 ? "Enabled" : "Disabled");
    if (tmp & 0x1) {
	printf("DACA en:%08x source:%08x\n", tmp, GET_REG(EVERGREEN_DACA_BASE + AVIVO_DAC_SOURCE_SELECT_OFFSET));
    }

    tmp = GET_REG(EVERGREEN_DACB_BASE);
    printf("DACB: %s\n", tmp & 0x1 ? "Enabled" : "Disabled");
    if (tmp & 0x1) {
	printf("DACB: en:%08x source:%08x\n", tmp, GET_REG(EVERGREEN_DACB_BASE + AVIVO_DAC_SOURCE_SELECT_OFFSET));
    }
    printf("\n");

    if (show_all) {
	for (i = 0x1410 ; i < 0x1600; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x2000 ; i < 0x2800; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x6250 ; i < 0x626c; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x6400 ; i < 0x641c; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x6420 ; i < 0x647c; i+=4)
	    SHOW_UNKNOWN_REG(i);
	SHOW_UNKNOWN_REG(0x6590);
	SHOW_UNKNOWN_REG(0x65b4);
	SHOW_UNKNOWN_REG(0x65cc);

	for (i = 0x6660 ; i <= 0x66dc; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x6800 ; i <= 0x6840; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x6998 ; i <= 0x69e0; i+=4)
	    SHOW_UNKNOWN_REG(i);

	for (i = 0x6a00 ; i < 0x6a1c; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x6b00 ; i < 0x6b1c; i+=4)
	    SHOW_UNKNOWN_REG(i);
	/* 6d00, 6d04 appear to change randomly */
	for (i = 0x6d08 ; i < 0x6d80; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x6df0 ; i < 0x6e7c; i+=4)
	    SHOW_UNKNOWN_REG(i);


	for (i = 0x8000 ; i <= 0xa000; i+=4)
	    SHOW_UNKNOWN_REG(i);
	for (i = 0x28000 ; i <= 0x2a000; i+=4)
	    SHOW_UNKNOWN_REG(i);
    }

    if (show_hdmi) {
	printf("\nAudio clocks:\n");
	SHOW_REG(EVERGREEN_AUDIO_PLL1_MUL);
	SHOW_REG(EVERGREEN_AUDIO_PLL1_DIV);
	SHOW_REG(EVERGREEN_AUDIO_PLL1_UNK);

	printf("\nAudio general:\n");
	SHOW_REG(EVERGREEN_AUDIO_ENABLE);

	printf("\nAudio params:\n");
	SHOW_REG(EVERGREEN_AUDIO_VENDOR_ID);

	printf("\nAudio verbs:\n");
	SHOW_REG(EVERGREEN_AUDIO_RATE_BPS_CHANNEL);
	SHOW_REG(EVERGREEN_AUDIO_PLAYING);
	SHOW_REG(EVERGREEN_AUDIO_CAT_STATUS);

	for (i = 0; i < EG_NUM_OFFSETS; i++) {
	    tmp = EG_GET_MODE_REG(i, EVERGREEN_DIG_CNTL);
	    if (!(tmp & 0x01000100)) /* Encoder not enabled */
		    continue;
	    tmp1 = (tmp & EVERGREEN_DIG_CNTL_ENC_MODE_MASK) >>
		    EVERGREEN_DIG_CNTL_ENC_MODE_SHIFT;
	    if (tmp1 != EVERGREEN_DIG_CNTL_ENC_MODE_DP &&
		tmp1 != EVERGREEN_DIG_CNTL_ENC_MODE_HDMI)
		    continue;

	    printf("\nHDMI block %d:\n", i);
	    for (j = 0; j < 0x100; j += 4) {
		tmp1 = EG_GET_MODE_REG(i, EVERGREEN_HDMI_BASE + j);
		printf("%08lx\t%08x (%d)\n", EG_MODE_REG_OFFSET(i, EVERGREEN_HDMI_BASE + j), tmp1, tmp1);
	    }
	}
    }
}