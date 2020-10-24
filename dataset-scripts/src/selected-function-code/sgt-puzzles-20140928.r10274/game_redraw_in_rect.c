static void game_redraw_in_rect(drawing *dr, game_drawstate *ds,
                                const game_state *state,
                                int x, int y, int w, int h)
{
    grid *g = state->game_grid;
    int i, phase;
    int bx, by, bw, bh;

    clip(dr, x, y, w, h);
    draw_rect(dr, x, y, w, h, COL_BACKGROUND);

    for (i = 0; i < g->num_faces; i++) {
        if (state->clues[i] >= 0) {
            face_text_bbox(ds, g, &g->faces[i], &bx, &by, &bw, &bh);
            if (boxes_intersect(x, y, w, h, bx, by, bw, bh))
                game_redraw_clue(dr, ds, state, i);
        }
    }
    for (phase = 0; phase < NPHASES; phase++) {
        for (i = 0; i < g->num_edges; i++) {
            edge_bbox(ds, g, &g->edges[i], &bx, &by, &bw, &bh);
            if (boxes_intersect(x, y, w, h, bx, by, bw, bh))
                game_redraw_line(dr, ds, state, i, phase);
        }
    }
    for (i = 0; i < g->num_dots; i++) {
        dot_bbox(ds, g, &g->dots[i], &bx, &by, &bw, &bh);
        if (boxes_intersect(x, y, w, h, bx, by, bw, bh))
            game_redraw_dot(dr, ds, state, i);
    }

    unclip(dr);
    draw_update(dr, x, y, w, h);
}