processPoint(Context, x, y)
struct Sundata * Context;
int x, y;      /* Screen co-ordinates of mouse */
{
    /*
     * Local Variables
     */

    City *city;    /* Used to search for a city */
    int cx, cy;    /* Screen coordinates of the city */

    /* Loop through the cities until on close to the pointer is found */

    for (city = cityroot; city; city = city->next) {

        /* Convert the latitude and longitude of the cities to integer */

        if (city->size == 0) continue;
        if (Context->zoom.width <
               Context->sizelimits[city->size-1] && city!=cityinit) continue;
        cx = int_longitude(Context, city->lon)-x;
	cy = int_latitude(Context, city->lat)-y;

        /* Check to see if we are close enough */

        if (cx*cx+cy*cy <= 13) break;
    }

    if (Context->flags.map_mode == LEGALTIME) {
      if (city)
        Context->flags.map_mode = COORDINATES;
      else
        Context->flags.map_mode = SOLARTIME;
    }

    updateUrban(Context, city);

    switch(Context->flags.map_mode) {

      case COORDINATES:
      case EXTENSION:
        if (city)
           Context->mark1.city = city;
        Context->flags.update = 1;
        break;

      case DISTANCES:
        if (Context->mark2.city) {
	    if (Context->flags.colorlevel==FULLCOLORS) {
	       erase_obj = 2;
	       drawMarks(Context);
               erase_obj = 0;
	    }
	}
        if (Context->mark1.city == &Context->pos1) {
            Context->pos2 = Context->pos1;
            Context->mark2.city = &Context->pos2;
        } else
            Context->mark2.city = Context->mark1.city;
        if (city)
           Context->mark1.city = city;
        else
           setPosition1(Context, x, y);
        Context->flags.update = 2;
        break;

      case SOLARTIME:
        if (Context->mark1.city) {
	   if (Context->flags.colorlevel==FULLCOLORS) {
	      erase_obj = 1;
	      drawMarks(Context);
              erase_obj = 0;
	   }
	}
        if (city)
           Context->mark1.city = city;
        else
           setPosition1(Context, x, y);
        Context->flags.update = 2;
        break;

      default:
        break;
    }

    setDayParams(Context);

    if (Context->flags.colorlevel==MONOCHROME) {
      if (Context->mark1.city) Context->mark1.flags = -1;
      if (Context->mark2.city) Context->mark2.flags = -1;
    } else {
      drawAll(Context);
      showMapImage(Context);
      Context->flags.update = 2;
    }

    if (do_urban && !city) updateUrban(Context, Context->mark1.city);
}
