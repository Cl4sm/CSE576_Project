he_again(struct he_s *hedit, long position)
  /* Walk backwards through the undo list, until a command is found with
   * the `again' flag set and reperfom this command on position `position'.
   * If no such command is found, a beep is "returned".
   */
{
  struct he_command_s *command;

  if (hedit->swapping) {
    long swap_position = ftell(hedit->undo.swapfile);
    for (command = 0;;) {
      long p;
      p = he_rewind_command(hedit);
      if (p < 0) {
        command = 0;
        break;
      }
      if (command) he_free_command(command);
      command = he_read_command(hedit);
      if (command->again) break;
    }
    if (!command)
      tio_bell();
    else {
      struct he_command_s *i;
      for (i = command; i; i = i->next_subcommand) i->position = position;
      he_do_command(hedit, command);
      he_compound_comand(hedit, command);
    }
    fseek(hedit->undo.swapfile, swap_position, SEEK_SET);
  } else {
    struct he_command_s *i;
    for (i = hedit->undo.list, command = 0; i; i = i->next_command)
      if (i->again) command = i;
    if (!command)
      tio_bell();
    else {
      /* We have to copy the command, because we want to change the
       * positions.  If `command' is an insert command, the field `data'
       * will be copied to `command2'.  If `command' is a delete command,
       * we'll read the `data' field from the buffer.  In this case it may
       * be nesseccary to adjust `j->count' in `command2'.
       */
      struct he_command_s *command2, *j;
      command2 = (struct he_command_s *)malloc(sizeof(struct he_command_s));
      for (i = command, j = command2; i;) {
        *j = *i;
        j->next_command = 0;
        j->prev_command = 0;
        if (i->count) {
          j->data = (char *)malloc(i->count);
          if (i->type) /* insert */
            memcpy(j->data, i->data, i->count);
          else { /* delete */
            long x = b_read(hedit->buffer, j->data, position, j->count);
            if (x < j->count) {
              /* `x > j->count' is impossible here. */
              j->data = (char *)realloc(j->data, x);
              j->count = x;
            }
          }
        }
        if (i->next_subcommand) {
          j->next_subcommand =
            (struct he_command_s *)malloc(sizeof(struct he_command_s));
          j = j->next_subcommand;
          i = i->next_subcommand;
        } else
          i = j->next_subcommand = 0;
      }
      for (i = command2; i; i = i->next_subcommand) i->position = position;
      he_do_command(hedit, command2);
      he_compound_comand(hedit, command2);
      /* The memory allocated by `command2' will not be freed here,
       * because it has been inserted into the undo list.
       */
    }
  }
}
