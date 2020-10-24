static void lines_redraw(drawing *dr, const game_state *state,
                         game_drawstate *ds, const game_ui *ui,
                         int x, int y, grid_type v, int lv, int lh)
{
    int ox = COORD(x), oy = COORD(y);
    int vcol = (v & G_FLASH) ? COL_HIGHLIGHT :
        (v & G_WARN) ? COL_WARNING : COL_FOREGROUND, hcol = vcol;
    grid_type todraw = v & G_NOLINE;

    if (v & G_ISSEL) {
        if (ui->todraw & G_FLAGSH) hcol = COL_SELECTED;
        if (ui->todraw & G_FLAGSV) vcol = COL_SELECTED;
        todraw |= ui->todraw;
    }

    draw_rect(dr, ox, oy, TILE_SIZE, TILE_SIZE, COL_BACKGROUND);
    /*if (v & G_CURSOR)
        draw_rect(dr, ox+TILE_SIZE/4, oy+TILE_SIZE/4,
                  TILE_SIZE/2, TILE_SIZE/2, COL_CURSOR);*/


    if (ui->show_hints) {
        if (between_island(state, x, y, 0, 1) && !(v & G_LINEV))
            vcol = COL_HINT;
        if (between_island(state, x, y, 1, 0) && !(v & G_LINEH))
            hcol = COL_HINT;
    }
#ifdef DRAW_GRID
    draw_rect_outline(dr, ox, oy, TILE_SIZE, TILE_SIZE, COL_GRID);
#endif

    if (todraw & G_NOLINEV) {
        line_cross(dr, ds, ox + TS8(3), oy + TS8(1), vcol, todraw);
        line_cross(dr, ds, ox + TS8(3), oy + TS8(5), vcol, todraw);
    }
    if (todraw & G_NOLINEH) {
        line_cross(dr, ds, ox + TS8(1), oy + TS8(3), hcol, todraw);
        line_cross(dr, ds, ox + TS8(5), oy + TS8(3), hcol, todraw);
    }
    /* if we're drawing a real line and a hint, make sure we draw the real
     * line on top. */
    if (lv && vcol == COL_HINT) lines_vert(dr, ds, ox, oy, lv, vcol, v);
    if (lh) lines_horiz(dr, ds, ox, oy, lh, hcol, v);
    if (lv && vcol != COL_HINT) lines_vert(dr, ds, ox, oy, lv, vcol, v);

    dsf_debug_draw(dr, state, ds, x, y);
    draw_update(dr, ox, oy, TILE_SIZE, TILE_SIZE);
}