static void hint_redraw(drawing *dr, game_drawstate *ds, int guess,
                        pegrow src, int force, int cursor, int markable)
{
    pegrow dest = ds->guesses[guess];
    int rowx, rowy, i, scol, col, hintlen;
    int need_redraw;
    int emptycol = (markable ? COL_FLASH : COL_EMPTY);

    if (src) assert(src->npegs == dest->npegs);

    hintlen = (dest->npegs + 1)/2;

    /*
     * Because of the possible presence of the cursor around this
     * entire section, we redraw all or none of it but never part.
     */
    need_redraw = FALSE;

    for (i = 0; i < dest->npegs; i++) {
        scol = src ? src->feedback[i] : 0;
        if (i == 0 && cursor)
            scol |= 0x1000;
        if (i == 0 && markable)
            scol |= 0x2000;
        if ((scol != dest->feedback[i]) || force) {
            need_redraw = TRUE;
        }
        dest->feedback[i] = scol;
    }

    if (need_redraw) {
        int hinth = HINTSZ + GAP + HINTSZ;
        int hx,hy,hw,hh;

        hx = HINT_X(guess)-GAP; hy = HINT_Y(guess)-GAP;
        hw = HINT_W+GAP*2; hh = hinth+GAP*2;

        /* erase a large background rectangle */
        draw_rect(dr, hx, hy, hw, hh, COL_BACKGROUND);

        for (i = 0; i < dest->npegs; i++) {
            scol = src ? src->feedback[i] : 0;
            col = ((scol == FEEDBACK_CORRECTPLACE) ? COL_CORRECTPLACE :
                   (scol == FEEDBACK_CORRECTCOLOUR) ? COL_CORRECTCOLOUR :
                   emptycol);

            rowx = HINT_X(guess);
            rowy = HINT_Y(guess);
            if (i < hintlen) {
                rowx += HINTOFF * i;
            } else {
                rowx += HINTOFF * (i - hintlen);
                rowy += HINTOFF;
            }
            if (HINTRAD > 0) {
                draw_circle(dr, rowx+HINTRAD, rowy+HINTRAD, HINTRAD, col,
                            (col == emptycol ? emptycol : COL_FRAME));
            } else {
                draw_rect(dr, rowx, rowy, HINTSZ, HINTSZ, col);
            }
        }
        if (cursor) {
            int x1,y1,x2,y2;
            x1 = hx + CGAP; y1 = hy + CGAP;
            x2 = hx + hw - CGAP; y2 = hy + hh - CGAP;
            draw_line(dr, x1, y1, x2, y1, COL_CURSOR);
            draw_line(dr, x2, y1, x2, y2, COL_CURSOR);
            draw_line(dr, x2, y2, x1, y2, COL_CURSOR);
            draw_line(dr, x1, y2, x1, y1, COL_CURSOR);
        }

        draw_update(dr, hx, hy, hw, hh);
    }
}