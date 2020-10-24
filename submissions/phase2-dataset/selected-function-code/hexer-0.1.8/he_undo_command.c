  static long
he_undo_command(struct he_s *hedit, struct he_command_s *command)
  /* Undo the compound command `command'.  The return value is the position
   * of the last change made.
   */
{
  if (command->next_subcommand)
    he_undo_command(hedit, command->next_subcommand);
    /* The last subcommand of the given compound has to be undone first.
     */
  if (command->type) {
    /* An insert command, we have to delete.
     */
    b_delete(hedit->buffer, command->position, command->count);
    he_refresh_part(hedit, command->position, -1);
  } else {
    /* A delete command, we have to reinsert.
     */
    b_insert(hedit->buffer, command->position, command->count);
    b_write(hedit->buffer, command->data, command->position, command->count);
    he_refresh_part(hedit, command->position, -1);
  }
  return command->position;
}
