he_compound_comand(hedit, command)
  struct he_s *hedit;
  struct he_command_s *command;
{
  char *buf;
  size_t sz, pos;
#define BADD(ptr, size)	memcpy(buf + pos, (ptr), (size)); pos += (size);
#define BADDC(c)	buf[pos++] = (c);

  if (hedit->swapping) {
    /* Remember the file position of `hedit->undo.swapfile'.
     */
    struct he_command_s *i;
    long swap_position = ftell(hedit->undo.swapfile);
    int n;

    /* Write the command to the swapfile.
     */
    sz = 4 + 1 + 4 + 4;
    for (n = 0, i = command; i; ++n, i = i->next_subcommand)
      /* Count the number of subcommands and determine the size of the buffer.
       */
      sz += 1 + 4 + 4 + i->count;
    buf = alloca(sz);
    if (buf == NULL) {
      /* TODO: some kind of error indication */
      fclose(hedit->undo.swapfile);
      hedit->swapping = 0;
      return;
    }
    pos = 0;
    BADD(he_bigendian(n), 4);
    BADDC(command->again);
    for (i = command; i; i = i->next_subcommand) {
      /* Write the subcommands to the swap file.
       */
      BADDC(i->type);
      BADD(he_bigendian(i->position), 4);
      BADD(he_bigendian(i->count), 4);
      BADD(i->data, i->count);
    }
    BADD(he_bigendian(swap_position), 4);
    BADD("\0\0\0\0", 4);
    if (fwrite(buf, 1, pos, hedit->undo.swapfile) != pos ||
	fflush(hedit->undo.swapfile) == EOF ||
	fseek(hedit->undo.swapfile, -4, SEEK_CUR) == EOF) {
      /* TODO: some kind of error indication */
      fclose(hedit->undo.swapfile);
      hedit->swapping = 0;
      return;
    }
    he_free_command(command);
  } else { /* (!hedit->swapping) */
    if (!hedit->command) {
      hedit->command = command;
      if (hedit->undo.list) he_free_command(hedit->undo.list);
      hedit->undo.list = command;
    } else {
      if (hedit->command->next_command)
        he_free_command(hedit->command->next_command);
      hedit->command->next_command = command;
      command->prev_command = hedit->command;
      hedit->command = hedit->command->next_command;
    }
  } /* if */
}
