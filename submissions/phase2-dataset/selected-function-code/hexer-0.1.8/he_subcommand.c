he_subcommand(hedit, type, position, count, data)
  struct he_s *hedit;
  int type;
  long position;
  long count;
  char *data;
  /* Update the undo-list of `hedit' by inserting the given subcommand.
   * The command is *not* performed by calling `he_subcommand()'.
   * A sequence of subcommands must be terminated by calling
   * `he_subcommand(hedit, -1, 0, 0, 0)'.
   * NOTE:  Iff the position for all subcommands is the same, the `again'
   *   flag will be set to 1.
   */
{
  static struct he_command_s *command = 0;
    /* Maybe `command' will become part of `struct he_s' instead of being
     * remembered statcially.  To stay compatible `hedit' should be 
     * specified in all calls to `he_subcommand()'.
     */
  static long last_position;
  static int again;
  struct he_command_s *i;

  if (type < 0) {
    command->again = again;
    he_compound_comand(hedit, command);
    command = 0;
  } else { /* (type >= 0) */
    if (!command) {
      command = (struct he_command_s *)malloc(sizeof(*command));
      again = 1;
      last_position = position;
      i = command;
    } else {
      if (again) if (position != last_position) again = 0;
      for (i = command; i->next_subcommand; i = i->next_subcommand);
      i->next_subcommand = (struct he_command_s *)malloc(sizeof(*command));
      i = i->next_subcommand;
    }
    i->next_subcommand = 0;
    i->next_command = 0;
    i->prev_command = 0;
    i->type = type;
    i->position = position;
    i->count = count;
    i->data = data;
  }
}
