static void trim_rect(int *x, int *y, int *w, int *h)
{
    int x0, x1, y0, y1;

    /*
     * Reduce the size of the copied rectangle to stop it going
     * outside the bounds of the canvas.
     */

    /* Transform from x,y,w,h form into coordinates of all edges */
    x0 = *x;
    y0 = *y;
    x1 = *x + *w;
    y1 = *y + *h;

    /* Clip each coordinate at both extremes of the canvas */
    x0 = (x0 < 0 ? 0 : x0 > canvas_w ? canvas_w : x0);
    x1 = (x1 < 0 ? 0 : x1 > canvas_w ? canvas_w : x1);
    y0 = (y0 < 0 ? 0 : y0 > canvas_h ? canvas_h : y0);
    y1 = (y1 < 0 ? 0 : y1 > canvas_h ? canvas_h : y1); 

    /* Transform back into x,y,w,h to return */
    *x = x0;
    *y = y0;
    *w = x1 - x0;
    *h = y1 - y0;
}