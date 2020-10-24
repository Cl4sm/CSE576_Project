
command *new_command(struct plotter *pl)
{
  command *c;
  
  c = (command *) malloc(sizeof(command));
  if (c == 0) fatalerror("malloc returned null");
  c->decoration = FALSE;
#ifdef WINDOW_COORDS_IN_COMMAND_STRUCT
  c->a.x = 0;
  c->a.y = 0;
  c->b.x = 0;
  c->b.y = 0;
#endif
  c->color = pl->current_color;
  
  c->xa.d = 0.0;
  c->ya.d = 0.0;
  c->xb.d = 0.0;
  c->yb.d = 0.0;

  c->next = pl->commands;
  pl->commands = c;

  return c;