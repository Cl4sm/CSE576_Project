static int unruly_validate_rows(const game_state *state, int horizontal,
                                char check, int *errors)
{
    int w2 = state->w2, h2 = state->h2;

    int dx = horizontal ? 1 : 0, dy = 1 - dx;

    int sx = dx, sy = dy;
    int ex = w2 - dx, ey = h2 - dy;

    int x, y;
    int ret = 0;

    int err1 = (horizontal ? FE_HOR_ROW_LEFT : FE_VER_ROW_TOP);
    int err2 = (horizontal ? FE_HOR_ROW_MID : FE_VER_ROW_MID);
    int err3 = (horizontal ? FE_HOR_ROW_RIGHT : FE_VER_ROW_BOTTOM);

    /* Check for any three in a row, and mark errors accordingly (if
     * required) */
    for (y = sy; y < ey; y++) {
        for (x = sx; x < ex; x++) {
            int i1 = (y-dy) * w2 + (x-dx);
            int i2 = y * w2 + x;
            int i3 = (y+dy) * w2 + (x+dx);

            if (state->grid[i1] == check && state->grid[i2] == check
                && state->grid[i3] == check) {
                ret++;
                if (errors) {
                    errors[i1] |= err1;
                    errors[i2] |= err2;
                    errors[i3] |= err3;
                }
            }
        }
    }

    return ret;
}