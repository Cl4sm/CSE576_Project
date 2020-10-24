int nextchar(void)
{
  int c, c2, ch;

  c = hqxchar();
  if (c == -1)
    return -1;

  switch (state86 & 0xff00)
    {
    case 0x0000:
      c2 = hqxchar();
      if (c2 == -1)
	return -1;

      ch = (c << 2) | (c2 >> 4);
      state86 = 0x0100 | (c2 & 0x0f);
      break;

    case 0x0100:
      ch = ((state86 & 0x0f) << 4) | (c >> 2);
      state86 = 0x0200 | (c & 0x03);
      break;

    case 0x0200:
      ch = ((state86 & 0x03) << 6) | c;
      state86 = 0;
      break;
    }

  return ch;
}
