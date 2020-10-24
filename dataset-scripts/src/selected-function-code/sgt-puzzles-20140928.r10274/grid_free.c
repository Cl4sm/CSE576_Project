void grid_free(grid *g)
{
    assert(g->refcount);

    g->refcount--;
    if (g->refcount == 0) {
        int i;
        for (i = 0; i < g->num_faces; i++) {
            sfree(g->faces[i].dots);
            sfree(g->faces[i].edges);
        }
        for (i = 0; i < g->num_dots; i++) {
            sfree(g->dots[i].faces);
            sfree(g->dots[i].edges);
        }
        sfree(g->faces);
        sfree(g->edges);
        sfree(g->dots);
        sfree(g);
    }
}