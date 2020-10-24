static void list_lights(game_state *state, int ox, int oy, int origin,
                        ll_data *lld)
{
    int x,y;

    lld->ox = lld->minx = lld->maxx = ox;
    lld->oy = lld->miny = lld->maxy = oy;
    lld->include_origin = origin;

    y = oy;
    for (x = ox-1; x >= 0; x--) {
        if (GRID(state, flags, x, y) & F_BLACK) break;
        if (x < lld->minx) lld->minx = x;
    }
    for (x = ox+1; x < state->w; x++) {
        if (GRID(state, flags, x, y) & F_BLACK) break;
        if (x > lld->maxx) lld->maxx = x;
    }

    x = ox;
    for (y = oy-1; y >= 0; y--) {
        if (GRID(state, flags, x, y) & F_BLACK) break;
        if (y < lld->miny) lld->miny = y;
    }
    for (y = oy+1; y < state->h; y++) {
        if (GRID(state, flags, x, y) & F_BLACK) break;
        if (y > lld->maxy) lld->maxy = y;
    }
}