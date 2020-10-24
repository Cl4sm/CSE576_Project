static void draw_peg(drawing *dr, game_drawstate *ds, int cx, int cy,
		     int moving, int labelled, int col)
{
    /*
     * Some platforms antialias circles, which means we shouldn't
     * overwrite a circle of one colour with a circle of another
     * colour without erasing the background first. However, if the
     * peg is the one being dragged, we don't erase the background
     * because we _want_ it to alpha-blend nicely into whatever's
     * behind it.
     */
    if (!moving)
        draw_rect(dr, cx-CGAP, cy-CGAP, PEGSZ+CGAP*2, PEGSZ+CGAP*2,
                  COL_BACKGROUND);
    if (PEGRAD > 0) {
        draw_circle(dr, cx+PEGRAD, cy+PEGRAD, PEGRAD,
		    COL_EMPTY + col, (col ? COL_FRAME : COL_EMPTY));
    } else
        draw_rect(dr, cx, cy, PEGSZ, PEGSZ, COL_EMPTY + col);

    if (labelled && col) {
        char buf[2];
        buf[0] = 'a'-1 + col;
        buf[1] = '\0';
        draw_text(dr, cx+PEGRAD, cy+PEGRAD, FONT_VARIABLE, PEGRAD,
                  ALIGN_HCENTRE|ALIGN_VCENTRE, COL_FRAME, buf);
    }

    draw_update(dr, cx-CGAP, cy-CGAP, PEGSZ+CGAP*2, PEGSZ+CGAP*2);
}