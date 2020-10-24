  int numints;
  object const * obj;
  flt t = FHUGE;
  color col;

  numints=closest_intersection(&t, &obj, incident);
                /* find the number of intersections */
                /* and return the closest one.      */

  if (numints < 1) {
    /* if there weren't any object intersections then return the */
    /* black for the pixel color.                                */
    col.r = 0.0;
    col.g = 0.0;
    col.b = 0.0;

    return col;
  }

  col.r = 1.0;
  col.g = 1.0;
  col.b = 1.0;

  return col;
}
