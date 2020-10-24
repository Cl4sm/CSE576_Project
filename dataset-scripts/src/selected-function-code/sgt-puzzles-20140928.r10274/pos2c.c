void pos2c(int w, int h, int pos, int *cx, int *cy)
{
    int max = w+h+w+h;

    pos = (pos + max) % max;

    if (pos < w) {
        *cx = pos; *cy = -1; return;
    }
    pos -= w;
    if (pos < h) {
        *cx = w; *cy = pos; return;
    }
    pos -= h;
    if (pos < w) {
        *cx = w-pos-1; *cy = h; return;
    }
    pos -= w;
    if (pos < h) {
      *cx = -1; *cy = h-pos-1; return;
    }
    assert(!"invalid pos, huh?"); /* limited by % above! */
}