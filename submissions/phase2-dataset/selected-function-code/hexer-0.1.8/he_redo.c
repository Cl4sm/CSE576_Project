  static void
he_redo(struct he_s *hedit)
  /* Redo the last undo.
   */
{
  struct he_command_s *command;

  if (hedit->swapping) {
    command = he_read_command(hedit);
    if (!command)
      goto fail;
    else {
      hedit->position = he_do_command(hedit, command);
      he_free_command(command);
    }
  } else {
    if (!hedit->command)
      command = hedit->undo.list;
    else
      command = hedit->command->next_command;
    if (!command)
      goto fail;
    else {
      hedit->command = command;
      hedit->position = he_do_command(hedit, command);
    }
  }
  he_message(0, "redo");
  return;

fail:
  he_message(1, "nothing to redo");
}
