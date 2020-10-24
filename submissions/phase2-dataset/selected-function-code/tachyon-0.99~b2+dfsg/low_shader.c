  int numints;
  object const * obj;
  vector hit;
  flt t = FHUGE;

  numints=closest_intersection(&t, &obj, incident);
                /* find the number of intersections */
                /* and return the closest one.      */

  if (numints < 1) {
    /* if there weren't any object intersections then return the */
    /* background texture for the pixel color.                   */
    return incident->scene->bgtexfunc(incident);
  }

  RAYPNT(hit, (*incident), t) /* find the point of intersection from t */
  incident->opticdist = FHUGE; 
  return obj->tex->texfunc(&hit, obj->tex, incident);
}
