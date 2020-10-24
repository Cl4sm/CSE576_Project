renderIcon(char **data, Icon* pIcon, IconGC* igc)
{
    Pixmap d, mask;
    register int i, k ;
    int x, y ;
    GC maskgc, paintgc ;

    d = XCreatePixmap(Xdisplay, scrollBar.win, pIcon->width, pIcon->height, Xdepth);
    mask = XCreatePixmap(Xdisplay, scrollBar.win, pIcon->width, pIcon->height, 1);

    CheckIconGC( igc, d, mask );
    y = pIcon->origin_y ;

    for (i = 0; i < pIcon->height ; y++, i++ ) {
	x = pIcon->origin_x ;
        for (k = 0; k < pIcon->width ; k++, x++ ) {
	    maskgc = igc->maskGC ;
            switch (data[y][x]) {
             case ' ':
             case 'w': paintgc = igc->whiteGC ;  break;
             case '%':
             case 'd': paintgc = igc->darkGC ;  break;
             case '#':
             case 'b': paintgc = igc->blackGC ;  break;
             case '.':
             case 'l':
             default:  paintgc = grayGC;
	               maskgc = igc->maskGC_0 ;
		       break;
            }
            XDrawPoint(Xdisplay, d, paintgc, k, i);
            XDrawPoint(Xdisplay, mask, maskgc, k, i);
        }
    }

    pIcon->icon = d ;
    pIcon->icon_mask = mask ;
}
