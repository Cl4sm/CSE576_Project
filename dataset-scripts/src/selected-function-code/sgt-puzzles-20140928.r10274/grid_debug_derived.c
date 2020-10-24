static void grid_debug_derived(grid *g)
{
#ifdef DEBUG_GRID
    /* edges */
    int i;
    printf("--- Derived Grid Data ---\n");
    for (i = 0; i < g->num_edges; i++) {
        grid_edge *e = g->edges + i;
        printf("Edge %d: dots[%d,%d] faces[%d,%d]\n",
            i, (int)(e->dot1 - g->dots), (int)(e->dot2 - g->dots),
            e->face1 ? (int)(e->face1 - g->faces) : -1,
            e->face2 ? (int)(e->face2 - g->faces) : -1);
    }
    /* faces */
    for (i = 0; i < g->num_faces; i++) {
        grid_face *f = g->faces + i;
        int j;
        printf("Face %d: faces[", i);
        for (j = 0; j < f->order; j++) {
            grid_edge *e = f->edges[j];
            grid_face *f2 = (e->face1 == f) ? e->face2 : e->face1;
            printf("%s%d", j ? "," : "", f2 ? (int)(f2 - g->faces) : -1);
        }
        printf("]\n");
    }
    /* dots */
    for (i = 0; i < g->num_dots; i++) {
        grid_dot *d = g->dots + i;
        int j;
        printf("Dot %d: dots[", i);
        for (j = 0; j < d->order; j++) {
            grid_edge *e = d->edges[j];
            grid_dot *d2 = (e->dot1 == d) ? e->dot2 : e->dot1;
            printf("%s%d", j ? "," : "", (int)(d2 - g->dots));
        }
        printf("] faces[");
        for (j = 0; j < d->order; j++) {
            grid_face *f = d->faces[j];
            printf("%s%d", j ? "," : "", f ? (int)(f - g->faces) : -1);
        }
        printf("]\n");
    }
#endif
#ifdef SVG_GRID
    grid_try_svg(g, SVG_DOTS | SVG_EDGES | SVG_FACES);
#endif
}