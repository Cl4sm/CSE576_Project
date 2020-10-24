 * are desired.  This is a hint to new_plotter, which could check resources.
 * If tiling, we stack multiple windows vertically and make them smaller.
 */

void new_plotter(FILE *fp, Display *dpy, int numtiles, int tileno, int lineno)
{
  int r = 0;
  PLOTTER pl;

  do {
  
    pl = (PLOTTER) malloc(sizeof(*pl));
    if (pl == 0) fatalerror("malloc returned null");
    pl->next = the_plotter_list;
    the_plotter_list = pl;
  
    pl->dpy = dpy;
    pl->screen = XDefaultScreenOfDisplay(pl->dpy);

    pl->numtiles = numtiles;
    pl->tileno = tileno;

    pl->win = 0;

    pl->aspect_ratio = 0.0;
    pl->viewno = 0;
    pl->commands = NULL;
    pl->x_type = INT;
    pl->y_type = INT;
    pl->x_units = "";
    pl->y_units = "";
    pl->mainsize.x = 0;
    pl->mainsize.y = 0;
    pl->size_changed = 0;
    pl->size.x = 0;
    pl->size.y = 0;
    pl->origin.x = 0;
    pl->origin.y = 0;
    pl->state = NORMAL;
    pl->raw_dragstart.x = 0;
    pl->raw_dragstart.y = 0;
    pl->dragstart.x = 0;
    pl->dragstart.y = 0;
    pl->dragend.x = 0;
    pl->dragend.y = 0;
    pl->pointer.x = 0;
    pl->pointer.y = 0;
    pl->pointer_marks.x = 0;
    pl->pointer_marks.y = 0;
    pl->pointer_marks_on_screen = FALSE;
    pl->xplot_nagle_atom = None;
    pl->slave_draw_in_progress = FALSE;
    pl->slave_motion_pending = FALSE;
    pl->master_pointer.x = 0;
    pl->master_pointer.y = 0;
    pl->buttonsdown = 0;
    pl->new_expose = 0;
    pl->clean = 0;
    pl->default_color = -1;
    pl->current_color = -1;
    pl->thick = option_thick? TRUE: FALSE;

    r = get_input(fp, dpy, lineno, pl);