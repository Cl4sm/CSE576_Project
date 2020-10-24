     char *modes, *cmodes;
{
  int iout = 0;
  long bits = OPN;
  for (; *modes; modes++)
    switch (*modes) {
    case 'r': bits |= RDNG; goto outc;
    case 'w': case 'a': bits |= WRTNG; goto outc;
    case '+': bits |= (RDNG | WRTNG); goto outc;
    case 'b': bits |= BINARY; goto outc;
    case '0': bits |= BUF0; break;
    case '?': bits |= (TRACKED | CRDY); break;
    case 'x': bits |= EXCLUSIVE; break;
    outc: if (cmodes && (iout < 3)) cmodes[iout++] = *modes; break;
    }
  if (!cmodes) return bits;
  cmodes[iout] = 0;
  switch (cmodes[0]) {
  default: return 0;
  case 'r': case 'w': case 'a': return bits;
  }
}
