ReadColorMap(FILE *fd, int number, unsigned char buffer[3][MAXCOLORMAPSIZE],
	     int *gray)
{
    int i;
    unsigned char rgb[3];
    int flag;

    flag = TRUE;

    for (i = 0; i < number; ++i) {
	if (!ReadOK(fd, rgb, sizeof(rgb))) {
	    return 1;
	}
	buffer[CM_RED][i] = rgb[0];
	buffer[CM_GREEN][i] = rgb[1];
	buffer[CM_BLUE][i] = rgb[2];
	flag &= (rgb[0] == rgb[1] && rgb[1] == rgb[2]);
    }

#if 0
    if (flag)
	*gray = (number == 2) ? PBM_TYPE : PGM_TYPE;
    else
	*gray = PPM_TYPE;
#else
    *gray = 0;
#endif

    return FALSE;
}
