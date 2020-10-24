static void win_begin_puzzle(void *handle, float xm, float xc,
			     float ym, float yc, int pw, int ph, float wmm)
{
    frontend *fe = (frontend *)handle;
    int ppw, pph, pox, poy;
    float mmpw, mmph, mmox, mmoy;
    float scale;

    assert(fe->drawstatus != DRAWING);
    if (fe->drawstatus == NOTHING)
	return;

    ppw = GetDeviceCaps(fe->hdc, HORZRES);
    pph = GetDeviceCaps(fe->hdc, VERTRES);
    mmpw = (float)GetDeviceCaps(fe->hdc, HORZSIZE);
    mmph = (float)GetDeviceCaps(fe->hdc, VERTSIZE);

    /*
     * Compute the puzzle's position on the logical page.
     */
    mmox = xm * mmpw + xc;
    mmoy = ym * mmph + yc;

    /*
     * Work out what that comes to in pixels.
     */
    pox = (int)(mmox * (float)ppw / mmpw);
    poy = (int)(mmoy * (float)pph / mmph);

    /*
     * And determine the scale.
     * 
     * I need a scale such that the maximum puzzle-coordinate
     * extent of the rectangle (pw * scale) is equal to the pixel
     * equivalent of the puzzle's millimetre width (wmm * ppw /
     * mmpw).
     */
    scale = (wmm * ppw) / (mmpw * pw);

    /*
     * Now store pox, poy and scale for use in the main drawing
     * functions.
     */
    fe->printoffsetx = pox;
    fe->printoffsety = poy;
    fe->printpixelscale = scale;

    fe->linewidth = 1;
    fe->linedotted = FALSE;
}