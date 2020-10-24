  static void
he_delete(struct he_s *hedit, long count, int dont_save)
  /* If `dont_save == 1' the data deleted is not copied to the `kill_buffer'.
   */
{
  long start;
  char *data;

  if (hedit->begin_selection >= 0 &&
      hedit->end_selection >= hedit->begin_selection) {
    start = hedit->begin_selection;
    count = hedit->end_selection - start + 1;
    he_cancel_selection(hedit);
    he_update_screen(hedit);
  } else {
    start = hedit->position;
    if (count < 1) count = 1;
    if (start + count > hedit->buffer->size)
      count = hedit->buffer->size - start;
  }
  if (!count) return;
  data = (char *)malloc(count);
  b_read(hedit->buffer, data, start, count);
  if (!dont_save) {
    if (!kill_buffer)
      kill_buffer = new_buffer(0);
    else
      b_clear(kill_buffer);
    b_insert(kill_buffer, 0, count);
    b_write(kill_buffer, data, 0, count);
  }
  he_subcommand(hedit, 0, start, count, data);
  he_subcommand(hedit, -1, 0, 0, 0);
  b_delete(hedit->buffer, start, count);
  if (hedit->position > start) hedit->position -= count - 1;
  he_refresh_part(hedit, start, hedit->buffer->size + count - 1);

  if (count > 1)
    he_message(0, "0x%lx (%li) bytes deleted%s.", count, count,
               dont_save ? "" : " (cut)");
}
