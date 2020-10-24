he_line(const struct he_s *hedit, long position)
  /* Create a line suitable for `tio_display()' (including @-commands).
   * `position' ist the number of the first byte in the line to be
   * created.  If `hedit->insert_position' is a positive value and
   * placed in the created line, the place for this position will be left
   * blank in the hex view and replaced by a `?' in the text view.
   * The return value is a pointer to the created line.
   */
{
  static char line[256];
  char buf[16];
  char *p = line;
  int i, bytes_read;
  int inverse = 0;

  if (position > (long)hedit->buffer->size) {
    strcpy(line, "~@C");
    return line;
  }
  if (position >= 0) {
    sprintf(p, " %08lx:", position);
    bytes_read = b_read(hedit->buffer, buf, position, 16);
  } else {
    sprintf(p, "-%08lx:", -position);
    bytes_read = b_read(hedit->buffer, buf - position, 0, 16 + position)
                 - position;
  }
  p += strlen(p);
  for (i = 0; i < 16; ++i) {
    if (hedit->begin_selection >= 0
        && position + i > hedit->end_selection
        && inverse) {
      strcpy(p, "@~");
      p += 2;
      inverse = 0;
    }
    if (!(i % 8)) {
      if (inverse) {
        strcpy(p, "@~  ");
        p += 4;
      } else {
        *p++ = ' ';
        *p++ = ' ';
      }
      inverse = 0;
    } else
      *p++ = ' ';
    if (hedit->begin_selection >= 0
        && position + i >= hedit->begin_selection
        && position + i <= hedit->end_selection
        && hedit->end_selection >= hedit->begin_selection) {
      if (!inverse) {
        strcpy(p, "@Ar");
        p += 3;
        inverse = 1;
      }
    } else
      if (inverse) {
        strcpy(p, "@~");
        p += 2;
        inverse = 0;
      }
    if (position + i < 0 || i >= bytes_read)
      strcpy(p, "--");
    else if (hedit->insert_position >= 0
             && position + i == hedit->insert_position)
      sprintf(p, "  ");
#ifdef HE_UNDERLINE_BEGIN_SELECTION
    else if (position + i == hedit->begin_selection) {
      sprintf(p, "@U%02x@~", (unsigned char)buf[i]);
      if (inverse) {
        p += strlen(p);
        sprintf(p, "@Ar");
      }
    }
#endif
    else 
      sprintf(p, "%02x", (unsigned char)buf[i]);
    p += strlen(p);
  }
  if (inverse) {
    strcpy(p, "@~  ");
    p += 4;
  } else {
    *p++ = ' ';
    *p++ = ' ';
  }
  inverse = 0;
  for (i = 0; i < 16; ++i) {
    if (hedit->begin_selection >= 0
        && position + i >= hedit->begin_selection
        && position + i <= hedit->end_selection
        && hedit->end_selection >= hedit->begin_selection) {
      if (!inverse) {
        strcpy(p, "@Ar");
        p += 3;
        inverse = 1;
      }
    } else
      if (inverse) {
        strcpy(p, "@~");
        p += 2;
        inverse = 0;
      }
    if (position + i < 0 || i >= bytes_read)
      *p++ = '-';
    else if (hedit->insert_position >= 0
             && position + i == hedit->insert_position)
      *p++ = '?';
#ifdef HE_UNDERLINE_BEGIN_SELECTION
    else if (position + i == hedit->begin_selection) {
      sprintf(p, "@U%s%c@~", buf[i] == '@' ? "@" : "",
              tio_isprint(buf[i]) ? buf[i] : HE_ANYCHAR);
      p += strlen(p);
      if (inverse) { strcpy(p, "@Ar"); p += 3; }
    }
#endif
    else {
      if (buf[i] == '@') *p++ = '@';
      *p++ = tio_isprint(buf[i]) ? buf[i] : HE_ANYCHAR;
    }
  }
  if (inverse) {
    strcpy(p, "@~");
    p += 2;
  }
  *p = 0;
  return line;
}
