{
  unsigned line_h = Height2Pixel(1)-1;
  register unsigned hy = (line_h+1)*num, y = Row2Pixel(row);

    if(!TermWin.bMapped) return ;

    if (y < TermWin_internalBorder+line_h)
    {y = 0; hy += TermWin_internalBorder;}
    if (y + hy > (TermWin.height+TermWin_internalBorder)-line_h)
	hy = (TermWin.height-y)+TermWin_internalBorders;
    XClearArea(Xdisplay, drawBuffer, 0, y, TermWin.width+TermWin_internalBorders, hy, 0);
#if defined(BACKGROUND_IMAGE) || defined(TRANSPARENT)
    if (TermWin.tintGC)
    {
        XFillRectangle(Xdisplay, drawBuffer, TermWin.tintGC,
                       0, y, TermWin.width+TermWin_internalBorders, hy);
    }
#endif
}
