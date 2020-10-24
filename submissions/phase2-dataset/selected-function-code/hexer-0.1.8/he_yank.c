he_yank(struct he_s *hedit, long count)
{
  long start;

  if (kill_buffer) b_clear(kill_buffer); else kill_buffer = new_buffer(0);
  if (hedit->begin_selection >= 0 &&
      hedit->end_selection >= hedit->begin_selection) {
    start = hedit->begin_selection;
    count = hedit->end_selection - start + 1;
    he_cancel_selection(hedit);
  } else {
    start = hedit->position;
    if (count < 1) count = 1;
  }
  b_insert(kill_buffer, 0, count);
  b_copy(kill_buffer, hedit->buffer, 0, start, count);
  if (count > 1)
    he_message(0, "0x%lx (%li) bytes yanked.", count, count);
}
