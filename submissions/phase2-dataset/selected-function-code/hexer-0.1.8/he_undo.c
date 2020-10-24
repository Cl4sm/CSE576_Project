  static void
he_undo(struct he_s *hedit)
  /* Undo the last command.
   */
{
  struct he_command_s *command;

  if (hedit->swapping) {
    long position;
    position = he_rewind_command(hedit);
    if (position < 0)
      goto fail;
    else {
      command = he_read_command(hedit);
      fseek(hedit->undo.swapfile, position, SEEK_SET);
      hedit->position = he_undo_command(hedit, command);
      he_free_command(command);
      /* if this was the first command in the undo list, we'll clear the
       * modified-flag.
       */
      position = ftell(hedit->undo.swapfile);
      if (he_rewind_command(hedit) < 0)
        hedit->buffer->modified = 0;
      else
        fseek(hedit->undo.swapfile, position, SEEK_SET);
    }
  } else {
    if (!hedit->command)
      goto fail;
    else {
      hedit->position = he_undo_command(hedit, hedit->command);
      hedit->command = hedit->command->prev_command;
      if (!hedit->command) hedit->buffer->modified = 0;
    }
  }
  he_message(0, "undo");
  return;

fail:
  he_message(1, "nothing to undo");
}
