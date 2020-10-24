static unsigned int tile_flags(game_drawstate *ds, const game_state *state,
                               const game_ui *ui, int x, int y, int flashing)
{
    unsigned int flags = GRID(state, flags, x, y);
    int lights = GRID(state, lights, x, y);
    unsigned int ret = 0;

    if (flashing) ret |= DF_FLASH;
    if (ui && ui->cur_visible && x == ui->cur_x && y == ui->cur_y)
        ret |= DF_CURSOR;

    if (flags & F_BLACK) {
        ret |= DF_BLACK;
        if (flags & F_NUMBERED) {
#ifdef HINT_NUMBERS
            if (number_wrong(state, x, y))
		ret |= DF_NUMBERWRONG;
#endif
            ret |= DF_NUMBERED;
        }
    } else {
#ifdef HINT_LIGHTS
        if (lights > 0) ret |= DF_LIT;
#endif
        if (flags & F_LIGHT) {
            ret |= DF_LIGHT;
#ifdef HINT_OVERLAPS
            if (lights > 1) ret |= DF_OVERLAP;
#endif
        }
        if (flags & F_IMPOSSIBLE) ret |= DF_IMPOSSIBLE;
    }
    return ret;
}