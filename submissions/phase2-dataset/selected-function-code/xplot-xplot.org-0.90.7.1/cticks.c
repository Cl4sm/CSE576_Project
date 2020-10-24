void cticks(coord_type ctype, coord first, coord last, int horizontal,
	    void (*pp)(coord c, int labelflag))
{
  int level;
  int sublevel;
  coord at;
  coord step;
  int count;
  int subcount;
  int maxextrasubticks;

#define MAXTICKS 6

  if (horizontal) {
    maxextrasubticks = MAXTICKS + 1;
  } else {
    maxextrasubticks = ( MAXTICKS + 1 ) * 2;
  }
  /* start with the smallest tick level */
  for (level = 0; ; level++) {
    step = impls[(int)ctype]->tick(level);
    at = impls[(int)ctype]->round_up(first, step);
    
    for (count=1; count <= MAXTICKS; count++) {
      at = impls[(int)ctype]->add(at, step);
      if (impls[(int)ctype]->cmp(at, last) > 0)
	break;
    } /* count is now number of ticks that fit */

    /* if <=  max allowed, use this level */
    if (count <= MAXTICKS) break;
  }

  step = impls[(int)ctype]->tick(level);
    
  at = impls[(int)ctype]->round_up(first, step);

  while (impls[(int)ctype]->cmp(at, last) <= 0) {
    pp(at,1);
    at = impls[(int)ctype]->add(at, step);
  }
  sublevel = impls[(int)ctype]->subtick(level);
  if (sublevel != level) {

    step = impls[(int)ctype]->tick(sublevel);
    at = impls[(int)ctype]->round_up(first, step);
    
    for (subcount = 1; subcount - count <= maxextrasubticks; subcount++) {
      at = impls[(int)ctype]->add(at, step);
      if (impls[(int)ctype]->cmp(at, last) > 0)
	break;
    }
    
    if (subcount - count <= maxextrasubticks) {
      at = impls[(int)ctype]->round_up(first, step);
      while (impls[(int)ctype]->cmp(at, last) <= 0) {
	pp(at,0);
	at = impls[(int)ctype]->add(at, step);
      }
    }
  }

      
}