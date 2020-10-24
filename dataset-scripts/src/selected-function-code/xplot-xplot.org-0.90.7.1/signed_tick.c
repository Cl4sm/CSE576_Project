coord signed_tick(int level)
{
  coord r;

  r.i = 1;
  while (level >= 3) {
    r.i *= 10;
    level -= 3;
  }
  
  switch (level) {
  case 2:
    r.i *= 5;
    level -= 2;
    break;
  case 1:
    r.i *= 2;
    level -= 1;
    break;
  case 0:
    break;
  }

  return r;
}