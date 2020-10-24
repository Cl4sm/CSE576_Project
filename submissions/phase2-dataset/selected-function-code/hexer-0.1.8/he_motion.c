  static void
he_motion(struct he_s *hedit, int key, long count)
{
  int i;

  if (count <= 0) count = 1;
  switch (key) {
  case 'k':
  case HXKEY_UP:
    hedit->position -= count << 4;
    if (hedit->position < 0) hedit->position &= 0xf;
    break;
  case 'j':
  case HXKEY_DOWN:
    hedit->position += count << 4;
    if (hedit->position > (long)hedit->buffer->size)
      hedit->position = (long)hedit->buffer->size;
    break;
  case 'h':
  case HXKEY_LEFT:
    hedit->position -= count;
    if (hedit->position < 0) hedit->position = 0;
    break;
  case 'l':
  case HXKEY_RIGHT:
    hedit->position += count;
    if (hedit->position > hedit->buffer->size)
      hedit->position = hedit->buffer->size;
    break;
  case 'f' & 0x1f: /* C-f */
  case HXKEY_PAGE_DOWN:
    i = hedit->position;
    hedit->position += (hx_lines - 2) * 16;
    if (hedit->position > hedit->buffer->size)
      hedit->position = hedit->buffer->size;
    hedit->screen_offset += ((hedit->position - i) / 16) * 16;
    if (hedit->position != i) he_refresh_all(hedit);
    break;
  case 'b' & 0x1f: /* C-b */
  case HXKEY_PAGE_UP:
    i = hedit->position;
    hedit->position -= (hx_lines - 2) * 16;
    if (hedit->position < 0) hedit->position = 0;
    hedit->screen_offset -= ((i - hedit->position) / 16) * 16;
    while (hedit->screen_offset < -15) hedit->screen_offset += 16;
    if (hedit->position != i) he_refresh_all(hedit);
    break;
  case 'd' & 0x1f: /* C-d */
    i = hedit->position;
    hedit->position += ((hx_lines - 1) / 2) * 16;
    if (hedit->position > hedit->buffer->size)
      hedit->position = hedit->buffer->size;
    hedit->screen_offset += ((hedit->position - i) / 16) * 16;
    if (hedit->position != i) he_refresh_all(hedit);
    break;
  case 'u' & 0x1f: /* C-u */
    i = hedit->position;
    hedit->position -= ((hx_lines - 1) / 2) * 16;
    if (hedit->position < 0) hedit->position = 0;
    hedit->screen_offset -= ((i - hedit->position) / 16) * 16;
    while (hedit->screen_offset < -15) hedit->screen_offset += 16;
    if (hedit->position != i) he_refresh_all(hedit);
    break;
  case '>':
    count &= 0xf;
    while (count--) {
      --hedit->screen_offset;
      if (hedit->screen_offset < -15) hedit->screen_offset = 0;
    }
    he_refresh_part(hedit, 0, -1);
    break;
  case '<':
    count &= 0xf;
    while (count--) ++hedit->screen_offset;
    he_refresh_part(hedit, 0, -1);
    break;
  case HXKEY_TAB:
    hedit->text_mode = !hedit->text_mode;
    break;
  }
}
