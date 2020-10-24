
}

void size_window(struct plotter *pl)
{
  command *c;

  pl->origin.x = 70;
  pl->origin.y = 30;
  pl->size.x = pl->mainsize.x - pl->origin.x - 10;
  pl->size.y = pl->mainsize.y - pl->origin.y - 30;
  
  /*************** CAVEAT abstraction violation in emit_PS() code below,
    caused mostly by hardwired constants above. */
  while (pl->commands && pl->commands->decoration) {
    c = pl->commands;
    pl->commands = pl->commands->next;
    free_command(c);
  }

  axis(pl);
  
  for (c = pl->commands; c != NULL; c = c->next)
    compute_window_coords(pl, c);