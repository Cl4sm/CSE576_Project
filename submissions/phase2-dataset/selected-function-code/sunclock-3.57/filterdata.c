void
filterdata()
{
int i, j, t, ind;

    if (fillmode==0) return;

    if (fillmode==1) {

      for (j=0; j<mapheight; j++)
        for (i=0; i<=mapwidth; i++) {
            ind = i*mapheight+j;
	    t = grid[ind];
	    full = 1;
	    if ( (t & 16383) && !check(i,j,0) ) {
	       grid[ind] |= 16384;
	       continue;
	    }
	    full = 0;
	    t = t/65536;
	    if ((t>0) && !check(i,j,t)) grid[ind] |= 32768; 
        }

      for (j=0; j<mapheight; j++)
        for (i=0; i<=mapwidth; i++) {
            ind = i*mapheight+j;
	    t = grid[ind];
	    if (t&16384) 
	      grid[ind] = (t&16383) * 65536;
	    else
	    if (t&32768) 
              grid[ind] = 0;
	    else
	      grid[ind] = t&MASK;
        }
      goto correct;
    }

    /* only remains fillmode = 2 */
    full = 1;

    for (j=0; j<mapheight; j++)
      for (i=0; i<=mapwidth; i++)
	  if (check(i,j,0)) grid[i*mapheight+j] &= MASK;

    for (j=0; j<mapheight; j++)
      for (i=0; i<=mapwidth; i++) {
          ind = i*mapheight+j;
	  t = (grid[ind] & 32767) * 65536;
	  grid[ind] |= t;
      }

 correct:
    if (map->zoom.width > map->geom.width && map->zoom.dx == 0) {
      for (j=0; j<mapheight; j++) grid[j+mapheight] = grid[j+2*mapheight];
    }
}
