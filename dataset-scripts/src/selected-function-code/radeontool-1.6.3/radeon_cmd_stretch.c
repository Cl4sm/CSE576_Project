void radeon_cmd_stretch(char *param)
{
    unsigned long fp_vert_stretch,fp_horz_stretch;

    if (!radeon_cntl_mem)
        exit(-1);
    fp_vert_stretch = radeon_get(RADEON_FP_VERT_STRETCH,"RADEON_FP_VERT_STRETCH");
    fp_horz_stretch = radeon_get(RADEON_FP_HORZ_STRETCH,"RADEON_FP_HORZ_STRETCH");
    if(param == NULL) {
        printf("The horizontal stretching looks %s\n",(fp_horz_stretch&(RADEON_HORZ_STRETCH_ENABLE))?"on":"off");
        printf("The vertical stretching looks %s\n",(fp_vert_stretch&(RADEON_VERT_STRETCH_ENABLE))?"on":"off");
        exit (-1);
    } else if(strncmp(param,"ver",3) == 0) {
        fp_horz_stretch &= ~ RADEON_HORZ_STRETCH_ENABLE;
        fp_vert_stretch |= RADEON_VERT_STRETCH_ENABLE;
    } else if(strncmp(param,"hor",3) == 0) {
        fp_horz_stretch |= RADEON_HORZ_STRETCH_ENABLE;
        fp_vert_stretch &= ~ RADEON_VERT_STRETCH_ENABLE;
    } else if(strcmp(param,"on") == 0) {
        fp_horz_stretch |= RADEON_HORZ_STRETCH_ENABLE;
        fp_vert_stretch |= RADEON_VERT_STRETCH_ENABLE;
    } else if(strcmp(param,"auto") == 0) {
        fp_horz_stretch |= RADEON_HORZ_AUTO_RATIO;
        fp_horz_stretch |= RADEON_HORZ_AUTO_RATIO_INC;
        fp_horz_stretch |= RADEON_HORZ_STRETCH_ENABLE;
        fp_vert_stretch |= RADEON_VERT_AUTO_RATIO_EN;
        fp_vert_stretch |= RADEON_VERT_STRETCH_ENABLE;
    } else if(strcmp(param,"manual") == 0) {
        fp_horz_stretch &= ~ RADEON_HORZ_AUTO_RATIO;
        fp_horz_stretch &= ~ RADEON_HORZ_AUTO_RATIO_INC;
        fp_horz_stretch |= RADEON_HORZ_STRETCH_ENABLE;
        fp_vert_stretch &= ~ RADEON_VERT_AUTO_RATIO_EN;
        fp_vert_stretch |= RADEON_VERT_STRETCH_ENABLE;
    } else if(strcmp(param,"off") == 0) {
        fp_horz_stretch &= ~ RADEON_HORZ_STRETCH_ENABLE;
        fp_vert_stretch &= ~ RADEON_VERT_STRETCH_ENABLE;
    } else {
        usage();	    
    };
    radeon_set(RADEON_FP_HORZ_STRETCH,"RADEON_FP_HORZ_STRETCH",fp_horz_stretch);
    radeon_set(RADEON_FP_VERT_STRETCH,"RADEON_FP_VERT_STRETCH",fp_vert_stretch);
}