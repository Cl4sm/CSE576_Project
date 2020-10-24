static void win_blitter_load(void *handle, blitter *bl, int x, int y)
{
    frontend *fe = (frontend *)handle;
    HDC hdc_win, hdc_blit;
    HBITMAP prev_blit;

    assert(fe->drawstatus == DRAWING);

    assert(bl->bitmap); /* we should always have saved before loading */

    if (x == BLITTER_FROMSAVED) x = bl->x;
    if (y == BLITTER_FROMSAVED) y = bl->y;

    hdc_win = GetDC(fe->hwnd);
    hdc_blit = CreateCompatibleDC(hdc_win);

    prev_blit = SelectObject(hdc_blit, bl->bitmap);

    BitBlt(fe->hdc, x, y, bl->w, bl->h,
           hdc_blit, 0, 0, SRCCOPY);

    SelectObject(hdc_blit, prev_blit);
    DeleteDC(hdc_blit);
    ReleaseDC(fe->hwnd, hdc_win);
}