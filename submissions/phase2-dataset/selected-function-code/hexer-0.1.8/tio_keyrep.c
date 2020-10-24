tio_keyrep(key)
  int key;
{
  static char rep[8];
  int i;

  if (key < 0x100) {
    if (key <= 0x1f) {
      rep[0] = '^';
      rep[1] = (char)(key + '@');
      rep[2] = 0;
      return rep;
    }
    if (tio_isprint(key)) {
      rep[0] = (char)key;
      rep[1] = 0;
      return rep;
    }
  }
  for (i = 0; t_keys[i].key; ++i)
    if ((int)t_keys[i].key == key) return t_keys[i].srep;
  return "";
}
