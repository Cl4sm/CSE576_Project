void CLEAR_CHARS(unsigned x, unsigned y, unsigned num)
{
  unsigned char_w = Width2Pixel(1)-1, line_h = Height2Pixel(1)-1;
  register unsigned hx = (char_w+1)*num ,hy = line_h+1;

    if(!TermWin.bMapped) return ;

    if (x < TermWin_internalBorder+char_w) {x = 0; hx += TermWin_internalBorder;}
    if (y < TermWin_internalBorder+line_h) {y = 0; hy += TermWin_internalBorder ;}

    if (x + hx > (TermWin.width+TermWin_internalBorder) -char_w)
	hx = (TermWin.width+TermWin_internalBorders+1)-x;
    if (y + hy > (TermWin.height+TermWin_internalBorder) - line_h)
	hy = (TermWin.height+TermWin_internalBorders+1)-y;
    XClearArea(Xdisplay, drawBuffer, x, y, hx, hy, 0);
#if defined(BACKGROUND_IMAGE) || defined(TRANSPARENT)
    if (TermWin.tintGC)
    {
        XFillRectangle(Xdisplay, drawBuffer, TermWin.tintGC, x, y, hx, hy);
    }
#endif
}
