							pl->y_units);
  }
}
void axis(struct plotter *pl)
{
  command *com;

  com = new_command(pl);
  com->decoration = TRUE;
  com->type = LINE;
  com->xa = pl_x_left;
  com->ya = pl_y_top;
  com->xb = pl_x_left;
  com->yb = pl_y_bottom;

  com = new_command(pl);
  com->decoration = TRUE;
  com->type = LINE;
  com->xa = pl_x_left;
  com->ya = pl_y_bottom;
  com->xb = pl_x_right;
  com->yb = pl_y_bottom;

  the_plotter_we_are_working_on = pl;
  cticks(pl->x_type, pl_x_left, pl_x_right, 1, doxtick);