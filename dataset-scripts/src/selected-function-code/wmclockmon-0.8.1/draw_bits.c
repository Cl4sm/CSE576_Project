static void draw_bits(int num, int x, int y, int u, int d, int sx, int sy, int d1x, int d1y, int d2x, int d2y) {
    int v = num;
    int dx = u * (sx + d2x) + d1x;
    int dy = sy + d1y;
    int b[4], i = 0;
    int bx = 0;
    int by = 2 * style.bdigith + style.sdigith + 7 + d;

    if (num == 0) return;
    if (backlight == LIGHTON) bx = 50;

    b[3] = 0; b[2] = 0; b[1] = 0; b[0] = 0;
    while (v != 0) {
        if (i == 4) {
            fprintf(stderr, "Error : num is too big (%d)\n", num);
            exit(1);
        }
        b[i] = v % 2;
        v = v / 2;
        i++;
    }
    for (i = 0 ; i < 4 ; i++) {
        if (b[i]) {
            int px = x + dx;
            int py = y + (3 - i) * dy + d2y;
            dockapp_copyarea(parts, pixmap, bx, by, sx, sy, px, py);
        }
    }
}