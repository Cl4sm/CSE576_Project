static int vertex_degree(int w, int h, signed char *soln, int x, int y,
                         int anti, int *sx, int *sy)
{
    int ret = 0;

    assert(x >= 0 && x <= w && y >= 0 && y <= h);
    if (x > 0 && y > 0 && soln[(y-1)*w+(x-1)] - anti < 0) {
        if (sx) *sx = x-1;
        if (sy) *sy = y-1;
        ret++;
    }
    if (x > 0 && y < h && soln[y*w+(x-1)] + anti > 0) {
        if (sx) *sx = x-1;
        if (sy) *sy = y;
        ret++;
    }
    if (x < w && y > 0 && soln[(y-1)*w+x] + anti > 0) {
        if (sx) *sx = x;
        if (sy) *sy = y-1;
        ret++;
    }
    if (x < w && y < h && soln[y*w+x] - anti < 0) {
        if (sx) *sx = x;
        if (sy) *sy = y;
        ret++;
    }

    return anti ? 4 - ret : ret;
}