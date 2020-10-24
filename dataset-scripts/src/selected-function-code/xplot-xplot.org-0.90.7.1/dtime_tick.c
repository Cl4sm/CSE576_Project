coord
dtime_tick(int level)
{
  coord r;

  r.d = 1.0;

#define MIDLEVEL 60

  while ( level < MIDLEVEL - 3 ) {
    r.d /= 10.0;
    level += 3;
  }

  while (level < MIDLEVEL) {
    r.d /= 2.0;			/* to 5 */
    level++;
    if (level < MIDLEVEL) {
      r.d /= 2.5;		/* to 2 */
      level++;
    }
    if (level < MIDLEVEL) {
      r.d /= 2.0;		/* to 1 */
      level++;
    }
  }
    
  while ( level > MIDLEVEL + 3 ) {
    r.d *= 10.0;
    level -= 3;
  }

  while (level > MIDLEVEL)  {
    r.d *= 2.0;			/* to 2 */
    level--;
    if (level > MIDLEVEL)  {
      r.d *= 2.5;		/* to 5 */
      level--;
    }
    if (level > MIDLEVEL)  {
      r.d *= 2.0;		/* to 1 */
      level--;
    }
  }
  return r;
}