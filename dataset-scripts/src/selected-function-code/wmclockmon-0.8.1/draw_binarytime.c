static void draw_binarytime() {
    int sx = style.binzx;
    int sy = style.binzy;
    int d1x = style.bind1x;
    int d1y = style.bind1y;
    int d2x = style.bind2x;
    int d2y = style.bind2y;

    draw_bits(timeinfos->tm_hour / 10, style.binhx, style.binhy, 0, 0,
            sx, sy, d1x, d1y, d2x, d2y);
    draw_bits(timeinfos->tm_hour % 10, style.binhx, style.binhy, 1, 0,
            sx, sy, d1x, d1y, d2x, d2y);

    draw_bits(timeinfos->tm_min / 10, style.binmx, style.binmy, 0, 0,
            sx, sy, d1x, d1y, d2x, d2y);
    draw_bits(timeinfos->tm_min % 10, style.binmx, style.binmy, 1, 0,
            sx, sy, d1x, d1y, d2x, d2y);

    draw_bits(timeinfos->tm_sec / 10, style.binsx, style.binsy, 0, 0,
            sx, sy, d1x, d1y, d2x, d2y);
    draw_bits(timeinfos->tm_sec % 10, style.binsx, style.binsy, 1, 0,
            sx, sy, d1x, d1y, d2x, d2y);

    if (style.binix != -1) {
        int wd = timeinfos->tm_wday == 0 ? 7 : timeinfos->tm_wday;

        sx = style.binix;
        sy = style.biniy;
        d1x = style.bind3x;
        d1y = style.bind3y;
        d2x = style.bind4x;
        d2y = style.bind4y;

        draw_bits(wd, style.binwx, style.binwy, 0, 1,
            sx, sy, d1x, d1y, d2x, d2y);

        draw_bits(timeinfos->tm_mday / 10, style.bindx, style.bindy, 0, 1,
            sx, sy, d1x, d1y, d2x, d2y);
        draw_bits(timeinfos->tm_mday % 10, style.bindx, style.bindy, 1, 1,
            sx, sy, d1x, d1y, d2x, d2y);

        draw_bits((timeinfos->tm_mon + 1) / 10, style.binox, style.binoy, 0, 1,
            sx, sy, d1x, d1y, d2x, d2y);
        draw_bits((timeinfos->tm_mon + 1) % 10, style.binox, style.binoy, 1, 1,
            sx, sy, d1x, d1y, d2x, d2y);
    }
}