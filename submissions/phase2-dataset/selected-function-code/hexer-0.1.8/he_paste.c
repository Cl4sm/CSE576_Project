he_paste(struct he_s *hedit, long count)
{
  char *data;
  long x = count > 2 ? count : 1, c = -1;

  he_cancel_selection(hedit);
  if (kill_buffer ? !(c = kill_buffer->size) : 1) return;
  if (count < 1) count = 1;
  while (count--) {
    data = (char *)malloc(c);
    b_read(kill_buffer, data, 0, c);
    b_insert(hedit->buffer, hedit->position, c);
    b_write(hedit->buffer, data, hedit->position, c);
    he_subcommand(hedit, 1, hedit->position, c, data);
  }
  he_subcommand(hedit, -1, 0, 0, 0);
  he_refresh_part(hedit, hedit->position, -1);
  he_message(0, "0x%lx (%li) bytes pasted.", x * c, x * c);
}
