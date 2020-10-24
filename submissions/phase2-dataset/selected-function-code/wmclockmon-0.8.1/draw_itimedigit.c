static void draw_itimedigit() {
    int dx = 0, dy = 0, v1, v10, v100, v0, v00, nb, pc;
    int bw = style.bdigitw, bh = style.bdigith;
    int binc = style.bdigitw + 2;
    int tw = style.sdigitw, th = style.bdigith;
    int xd = 100;

    if (backlight == LIGHTON) {
        dx = 50;
        dy = 20;
        xd = 102;
    }

    /* use floor(3) */    
    v100 = swtime / 100.0;
    v10  = (swtime - v100 * 100) / 10.0;
    v1   = (swtime - v100 * 100.0 - v10 * 10.0);
    v0   = (swtime - v100 * 100.0 - v10 * 10.0 - v1) * 10;
    v00  = (swtime - v100 * 100.0 - v10 * 10.0 - v1) * 1000;
    pc   = v00 - v0 * 100;

    /* draw main beats digit */
    if (!style.bbig) {
        bw = tw;
        bh = th;
    }
    dockapp_copyarea(parts, pixmap, v1 * 10, dy, bw, bh, style.bposx + 2 * binc, style.bposy);
    dockapp_copyarea(parts, pixmap, v10 * 10, dy, bw, bh, style.bposx + binc, style.bposy);
    dockapp_copyarea(parts, pixmap, v100 * 10, dy, bw, bh, style.bposx, style.bposy);

    /* draw 10th of beats */
    dockapp_copyarea(parts, pixmap, v0 * 5 + dx, 40, 5, 9, 49, 28);

    /* graph */
    for (nb = 0 ; nb < pc / 6.25 ; nb++)
        dockapp_copyarea(parts, pixmap, xd, 0, 2, 9, 6 + nb * 3, 45);
}