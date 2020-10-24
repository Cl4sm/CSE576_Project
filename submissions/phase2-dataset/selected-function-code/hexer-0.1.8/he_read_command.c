  static struct he_command_s *
he_read_command(struct he_s *hedit)
  /* Read a compound command from the swap file.  If the end of the
   * swap file is hit (the read failed), the return value is 0, else
   * a pointer to the command read is returned.
   */
{
  struct he_command_s *command, *c;
  long n_subcommands;
  char bigendian[4];
  int i;

  if (fread(bigendian, 4, 1, hedit->undo.swapfile) != 1)
    return 0;
  n_subcommands = he_bigendian_to_long(bigendian);
  if (!n_subcommands) {
    fseek(hedit->undo.swapfile, -4, SEEK_CUR);
    return 0;
  }
  if (fgetc(hedit->undo.swapfile) == EOF)
    return 0;
  c = command = (struct he_command_s *)malloc(sizeof(struct he_command_s));
  for (i = 1; c; ++i) {
    int n;
    c->next_command = 0;
    c->prev_command = 0;
    n = fgetc(hedit->undo.swapfile);
    if (n == EOF)
      return 0;
    c->type = !!n;
    if (fread(bigendian, 4, 1, hedit->undo.swapfile) != 1)
      return 0;
    c->position = he_bigendian_to_long(bigendian);
    if (fread(bigendian, 4, 1, hedit->undo.swapfile) != 1)
      return 0;
    c->count = he_bigendian_to_long(bigendian);
    c->data = (char *)malloc(c->count);
    if (fread(c->data, 1, c->count, hedit->undo.swapfile) != c->count)
      return 0;
    if (i < n_subcommands) {
      c->next_subcommand =
        (struct he_command_s *)malloc(sizeof(struct he_command_s));
      c = c->next_subcommand;
    } else {
      c->next_subcommand = 0;
      c = 0;
    }
  }
  fseek(hedit->undo.swapfile, 4, SEEK_CUR);
  return command;
}
