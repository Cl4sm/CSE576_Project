static void guess_redraw(drawing *dr, game_drawstate *ds, int guess,
                         pegrow src, int *holds, int cur_col, int force,
                         int labelled)
{
    pegrow dest;
    int rowx, rowy, i, scol;

    if (guess == -1) {
        dest = ds->solution;
        rowx = SOLN_OX;
        rowy = SOLN_OY;
    } else {
        dest = ds->guesses[guess];
        rowx = GUESS_X(guess,0);
        rowy = GUESS_Y(guess,0);
    }
    if (src) assert(src->npegs == dest->npegs);

    for (i = 0; i < dest->npegs; i++) {
        scol = src ? src->pegs[i] : 0;
        if (i == cur_col)
            scol |= 0x1000;
        if (holds && holds[i])
            scol |= 0x2000;
        if (labelled)
            scol |= 0x4000;
        if ((dest->pegs[i] != scol) || force) {
	    draw_peg(dr, ds, rowx + PEGOFF * i, rowy, FALSE, labelled,
                     scol &~ 0x7000);
            /*
             * Hold marker.
             */
            draw_rect(dr, rowx + PEGOFF * i, rowy + PEGSZ + ds->gapsz/2,
                      PEGSZ, 2, (scol & 0x2000 ? COL_HOLD : COL_BACKGROUND));
            draw_update(dr, rowx + PEGOFF * i, rowy + PEGSZ + ds->gapsz/2,
                        PEGSZ, 2);
            if (scol & 0x1000)
                draw_cursor(dr, ds, rowx + PEGOFF * i, rowy);
        }
        dest->pegs[i] = scol;
    }
}