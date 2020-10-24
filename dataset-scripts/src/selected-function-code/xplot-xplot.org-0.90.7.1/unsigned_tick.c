coord unsigned_tick(int level)
{
  coord r;

  r.u = 1;
  while (level >= 3) {
    r.u *= 10;
    level -= 3;
  }

  switch (level) {
  case 2:
    r.u *= 5;
    level -= 2;
    break;
  case 1:
    r.u *= 2;
    level -= 1;
    break;
  case 0:
    break;
  }

  return r;
}