void radeon_cmd_regs(const char *type)
{

    /* Dump all registers that we can read. */
    if (strcmp(type, "radeon") == 0) {
	radeon_show_radeon_display_regs(type);
        return;
    }
    if (strcmp(type, "avivo") == 0) {
	radeon_show_avivo_display_regs(type);
        return;
    }
    if (strcmp(type, "dce3") == 0) {
	radeon_show_dce3_display_regs(type);
        return;
    }
    if (strcmp(type, "dce4") == 0) {
	radeon_show_dce4_display_regs(type);
        return;
    }
    if (strcmp(type, "dce5") == 0) {
	radeon_show_dce5_display_regs(type);
        return;
    }
    if (strcmp(type, "all") == 0) {
	    if (IS_DISPLAY_RADEON(card_info))
		    radeon_show_radeon_display_regs(type);
	    else if (IS_DISPLAY_AVIVO(card_info))
		    radeon_show_avivo_display_regs(type);
	    else if (IS_DISPLAY_DCE3(card_info))
		    radeon_show_dce3_display_regs(type);
	    else if (IS_DISPLAY_DCE4(card_info))
		    radeon_show_dce4_display_regs(type);
	    else if (IS_DISPLAY_DCE5(card_info))
		    radeon_show_dce5_display_regs(type);
	    else
		    printf("unknown chipset, specify the regs to dump\n");
	    return;
    }
}