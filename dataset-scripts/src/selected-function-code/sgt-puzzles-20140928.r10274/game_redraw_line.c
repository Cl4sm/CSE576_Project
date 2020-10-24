static void game_redraw_line(drawing *dr, game_drawstate *ds,
			     const game_state *state, int i, int phase)
{
    grid *g = state->game_grid;
    grid_edge *e = g->edges + i;
    int x1, x2, y1, y2;
    int line_colour;

    if (state->line_errors[i])
	line_colour = COL_MISTAKE;
    else if (state->lines[i] == LINE_UNKNOWN)
	line_colour = COL_LINEUNKNOWN;
    else if (state->lines[i] == LINE_NO)
	line_colour = COL_FAINT;
    else if (ds->flashing)
	line_colour = COL_HIGHLIGHT;
    else
	line_colour = COL_FOREGROUND;
    if (line_colour != loopy_line_redraw_phases[phase])
        return;

    /* Convert from grid to screen coordinates */
    grid_to_screen(ds, g, e->dot1->x, e->dot1->y, &x1, &y1);
    grid_to_screen(ds, g, e->dot2->x, e->dot2->y, &x2, &y2);

    if (line_colour == COL_FAINT) {
	static int draw_faint_lines = -1;
	if (draw_faint_lines < 0) {
	    char *env = getenv("LOOPY_FAINT_LINES");
	    draw_faint_lines = (!env || (env[0] == 'y' ||
					 env[0] == 'Y'));
	}
	if (draw_faint_lines)
	    draw_line(dr, x1, y1, x2, y2, line_colour);
    } else {
	draw_thick_line(dr, 3.0,
			x1 + 0.5, y1 + 0.5,
			x2 + 0.5, y2 + 0.5,
			line_colour);
    }
}