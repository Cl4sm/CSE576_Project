static void edge_bbox(game_drawstate *ds, grid *g, grid_edge *e,
                      int *x, int *y, int *w, int *h)
{
    int x1 = e->dot1->x;
    int y1 = e->dot1->y;
    int x2 = e->dot2->x;
    int y2 = e->dot2->y;
    int xmin, xmax, ymin, ymax;

    grid_to_screen(ds, g, x1, y1, &x1, &y1);
    grid_to_screen(ds, g, x2, y2, &x2, &y2);
    /* Allow extra margin for dots, and thickness of lines */
    xmin = min(x1, x2) - 2;
    xmax = max(x1, x2) + 2;
    ymin = min(y1, y2) - 2;
    ymax = max(y1, y2) + 2;

    *x = xmin;
    *y = ymin;
    *w = xmax - xmin + 1;
    *h = ymax - ymin + 1;
}