static void win_blitter_save(void *handle, blitter *bl, int x, int y)
{
    frontend *fe = (frontend *)handle;
    HDC hdc_win, hdc_blit;
    HBITMAP prev_blit;

    assert(fe->drawstatus == DRAWING);

    if (!bl->bitmap) blitter_mkbitmap(fe, bl);

    bl->x = x; bl->y = y;

    hdc_win = GetDC(fe->hwnd);
    hdc_blit = CreateCompatibleDC(hdc_win);
    if (!hdc_blit) fatal("hdc_blit failed: 0x%x", GetLastError());

    prev_blit = SelectObject(hdc_blit, bl->bitmap);
    if (prev_blit == NULL || prev_blit == HGDI_ERROR)
        fatal("SelectObject for hdc_main failed: 0x%x", GetLastError());

    if (!BitBlt(hdc_blit, 0, 0, bl->w, bl->h,
                fe->hdc, x, y, SRCCOPY))
        fatal("BitBlt failed: 0x%x", GetLastError());

    SelectObject(hdc_blit, prev_blit);
    DeleteDC(hdc_blit);
    ReleaseDC(fe->hwnd, hdc_win);
}