static void island_redraw(drawing *dr,
                          const game_state *state, game_drawstate *ds,
                          struct island *is, grid_type v)
{
    /* These overlap the edges of their squares, which is why they're drawn later.
     * We know they can't overlap each other because they're not allowed within 2
     * squares of each other. */
    int half = TILE_SIZE/2;
    int ox = COORD(is->x) + half, oy = COORD(is->y) + half;
    int orad = ISLAND_RADIUS, irad = orad - LINE_WIDTH;
    int updatesz = orad*2+1;
    int tcol = (v & G_FLASH) ? COL_HIGHLIGHT :
              (v & G_WARN)  ? COL_WARNING : COL_FOREGROUND;
    int col = (v & G_ISSEL) ? COL_SELECTED : tcol;
    int bg = (v & G_CURSOR) ? COL_CURSOR :
        (v & G_MARK) ? COL_MARK : COL_BACKGROUND;
    char str[32];

#ifdef DRAW_GRID
    draw_rect_outline(dr, COORD(is->x), COORD(is->y),
                      TILE_SIZE, TILE_SIZE, COL_GRID);
#endif

    /* draw a thick circle */
    draw_circle(dr, ox, oy, orad, col, col);
    draw_circle(dr, ox, oy, irad, bg, bg);

    sprintf(str, "%d", is->count);
    draw_text(dr, ox, oy, FONT_VARIABLE, ISLAND_NUMSIZE(is),
              ALIGN_VCENTRE | ALIGN_HCENTRE, tcol, str);

    dsf_debug_draw(dr, state, ds, is->x, is->y);
    draw_update(dr, ox - orad, oy - orad, updatesz, updatesz);
}