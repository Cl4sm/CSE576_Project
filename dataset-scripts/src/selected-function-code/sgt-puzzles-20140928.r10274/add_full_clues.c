static void add_full_clues(game_state *state, random_state *rs)
{
    signed char *clues = state->clues;
    grid *g = state->game_grid;
    char *board = snewn(g->num_faces, char);
    int i;

    generate_loop(g, board, rs, NULL, NULL);

    /* Fill out all the clues by initialising to 0, then iterating over
     * all edges and incrementing each clue as we find edges that border
     * between BLACK/WHITE faces.  While we're at it, we verify that the
     * algorithm does work, and there aren't any GREY faces still there. */
    memset(clues, 0, g->num_faces);
    for (i = 0; i < g->num_edges; i++) {
        grid_edge *e = g->edges + i;
        grid_face *f1 = e->face1;
        grid_face *f2 = e->face2;
        enum face_colour c1 = FACE_COLOUR(f1);
        enum face_colour c2 = FACE_COLOUR(f2);
        assert(c1 != FACE_GREY);
        assert(c2 != FACE_GREY);
        if (c1 != c2) {
            if (f1) clues[f1 - g->faces]++;
            if (f2) clues[f2 - g->faces]++;
        }
    }
    sfree(board);
}