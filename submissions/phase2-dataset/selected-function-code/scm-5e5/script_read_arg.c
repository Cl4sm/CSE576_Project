     FILE *f;
{
  sizet tlen = 1;
  int tind = 0, qted = 0, chr;
  char *tbuf = (char *)malloc((1 + tlen) * sizeof(char));
  if (!tbuf) return 0L;
  while (1) switch (chr = getc(f)) {
  case WHITE_SPACES:
    continue;
  case LINE_INCREMENTORS:
  case EOF:
    free(tbuf);
    return 0L;
  default:
    goto morearg;
  }
morearg:
  while (1) {
    switch (tbuf[tind++] = chr) {
    case WHITE_SPACES:
    case LINE_INCREMENTORS:
      if (qted) break;
    case EOF: goto endarg;
    case '!':
      if (qted) break;
      switch (chr = getc(f)) {
      case '#':
	if (1==tind) return 0L;
	goto endarg;
      default: tbuf[tind++] = chr; break;
      }
      break;
    case '"': qted = !qted; tind--; break;
    case '\\':
      switch (tbuf[tind - 1] = getc(f)) {
      case '\n': --tind; break;
      case 'n': tbuf[tind - 1] = '\n'; break;
      case 'r': tbuf[tind - 1] = '\r'; break;
      case 't': tbuf[tind - 1] = '\t'; break;
      case 'b': tbuf[tind - 1] = '\b'; break;
	/* case '0': tbuf[tind - 1] = '\0'; break; */
      default:;
      }
    default:;
    }
    if (tind >= tlen) {
      tbuf = (char *)realloc(tbuf, (1 + (2 * tlen)) * sizeof(char));
      if (!tbuf) return 0L;
      tlen = 2 * tlen;
    }
    chr = getc(f);
  }
endarg:
  tbuf[--tind] = 0;
  return tbuf;
}
