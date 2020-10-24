tio_keyname(key)
  /* Returns the name of the key `key'.  If `key' is a printable character,
   * it is returned as a string.  If `key' is a special key, the name of
   * that key is returned.  If `key' is unknown and greater than 0xff "??"
   * is returned, else a `\x??' hexadecimal code.
   */
  int key;
{
  int i;
  static char name[8];

  for (i = 0; t_keys[i].key; ++i)
    if ((int)t_keys[i].key == key) return t_keys[i].name;
  if (tio_isprint(key)) {
    name[0] = (char)key;
    name[1] = 0;
    return name;
  }
  if (key > 0xff) return "??";
  if (key == (int)HXKEY_ESCAPE) return HXKEY_ESCAPE_NAME;
  sprintf(name, "\\x%02x", (unsigned char)key);
  return name;
}
