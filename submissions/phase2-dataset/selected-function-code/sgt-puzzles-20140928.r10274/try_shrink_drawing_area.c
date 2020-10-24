static void try_shrink_drawing_area(frontend *fe)
{
    if (fe->drawing_area_shrink_pending &&
        !not_size_allocated_yet(fe->menubar) &&
        !not_size_allocated_yet(fe->statusbar)) {
        /*
         * In order to permit the user to resize the window smaller as
         * well as bigger, we call this function after the window size
         * has ended up where we want it. This shouldn't shrink the
         * window immediately; it just arranges that the next time the
         * user tries to shrink it, they can.
         *
         * However, at puzzle creation time, we defer the first of
         * these operations until after the menu bar and status bar
         * are actually visible. On Ubuntu 12.04 I've found that these
         * can take a while to be displayed, and that it's a mistake
         * to reduce the drawing area's size allocation before they've
         * turned up or else the drawing area makes room for them by
         * shrinking to less than the size we intended.
         */
        gtk_drawing_area_size(GTK_DRAWING_AREA(fe->area), 1, 1);
        fe->drawing_area_shrink_pending = FALSE;
    }
}