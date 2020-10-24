  static void
he_paste_over(struct he_s *hedit, long count)
{
  char *data_insert, *data_delete;
  long x = count > 2 ? count : 1, c = -1;
  long bytes_deleted;
  int i;

  he_cancel_selection(hedit);
  if (kill_buffer ? !(c = kill_buffer->size) : 1) return;
  if (count < 1) count = 1;

  data_insert = (char *)malloc(c * count);
  data_delete = (char *)malloc(c * count);
  b_read(kill_buffer, data_insert, 0, c);
  for (i = 1; i < count; ++i) memcpy(data_insert + c * i, data_insert, c);
  bytes_deleted = b_read(hedit->buffer, data_delete,
                         hedit->position, c * count);
  b_write_append(hedit->buffer, data_insert, hedit->position, c * count);
  if (bytes_deleted < c * count)
    data_delete =
      (char *)realloc(data_delete, bytes_deleted + !bytes_deleted);
  he_subcommand(hedit, 0, hedit->position, bytes_deleted, data_delete);
  he_subcommand(hedit, 1, hedit->position, c * count, data_insert);
  he_subcommand(hedit, -1, 0, 0, 0);
  he_refresh_part(hedit, hedit->position, -1);
  he_message(0, "0x%lx (%li) bytes replaced.", x * c, x * c);
}
