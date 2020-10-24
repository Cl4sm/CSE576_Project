void createXBMfromXPM(char *xbm, char **xpm, int sx, int sy) {

    int		i,j,k;
    int		width, height, numcol, depth;
    int 	zero=0;
    unsigned char	bwrite;
    int		bcount;
    int     curpixel;

    sscanf(*xpm, "%d %d %d %d", &width, &height, &numcol, &depth);


    for (k=0; k!=depth; k++) {
        zero <<=8;
        zero |= xpm[1][k];
    }

    for (i=numcol+1; i < numcol+sy+1; i++) {
        bcount = 0;
        bwrite = 0;
        for (j=0; j<sx*depth; j+=depth) {
            bwrite >>= 1;

            curpixel=0;
            for (k=0; k!=depth; k++) {
                curpixel <<=8;
                curpixel |= xpm[i][j+k];
            }

            if ( curpixel != zero ) {
                bwrite += 128;
            }
            bcount++;
            if (bcount == 8) {
                *xbm = bwrite;
                xbm++;
                bcount = 0;
                bwrite = 0;
            }
        }
    }
}