  void 
he_refresh_part(hedit, pos1, pos2)
  struct he_s *hedit;
  long pos1, pos2;
{
  long i, j, k;
  int update_f = 0;
  int lastline_f = 0;

  if (pos2 < 0) {
    pos2 = hedit->buffer->size - 1;
    ++lastline_f;
  }
  if (pos1 > pos2) i = pos1, pos1 = pos2, pos2 = i;
  j = HE_LINE(pos1 - hedit->screen_offset);
  k = lastline_f ? hx_lines - 2 : HE_LINE(pos2 - hedit->screen_offset);
  if (j < 0) {
    j = 0;
    if (k >= 0) {
      ++update_f;
      if (k > hx_lines - 2) k = hx_lines - 2;
    }
  } else {
    if (j <= hx_lines - 2) {
      ++update_f;
      if (k > hx_lines - 2) k = hx_lines - 2;
    }
  }
  if (update_f) he_refresh_lines(hedit, j, k);
}
