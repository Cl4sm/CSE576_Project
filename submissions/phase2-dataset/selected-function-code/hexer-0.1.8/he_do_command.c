he_do_command(hedit, command)
  struct he_s *hedit;
  struct he_command_s *command;
  /* Perform the compound command `command'.  The return value is the
   * position of the last change made.
   */
{
  if (command->type) {
    /* Insert.
     */
    b_insert(hedit->buffer, command->position, command->count);
    b_write(hedit->buffer, command->data, command->position, command->count);
    he_refresh_part(hedit, command->position, -1);
  } else {
    /* Delete.
     */
    b_delete(hedit->buffer, command->position, command->count);
    he_refresh_part(hedit, command->position, -1);
  }
  if (command->next_subcommand)
    return he_do_command(hedit, command->next_subcommand);
  return command->position;
}
