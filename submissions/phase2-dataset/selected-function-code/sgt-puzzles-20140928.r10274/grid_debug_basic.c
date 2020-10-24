static void grid_debug_basic(grid *g)
{
    /* TODO: Maybe we should generate an SVG image of the dots and lines
     * of the grid here, before grid_make_consistent.
     * Would help with debugging grid generation. */
#ifdef DEBUG_GRID
    int i;
    printf("--- Basic Grid Data ---\n");
    for (i = 0; i < g->num_faces; i++) {
        grid_face *f = g->faces + i;
        printf("Face %d: dots[", i);
        int j;
        for (j = 0; j < f->order; j++) {
            grid_dot *d = f->dots[j];
            printf("%s%d", j ? "," : "", (int)(d - g->dots)); 
        }
        printf("]\n");
    }
#endif
#ifdef SVG_GRID
    grid_try_svg(g, SVG_FACES);
#endif
}