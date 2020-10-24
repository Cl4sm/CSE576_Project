void add_clipped_intersection(flt t, const object * obj, ray * ry) {
  if (t > EPSILON) {
    /* if we hit something before maxdist update maxdist */
    if (t < ry->maxdist) {

      /* handle clipped object tests */
      if (obj->clip != NULL) {
        vector hit;
        int i;

        RAYPNT(hit, (*ry), t);    /* find the hit point for further tests */
        for (i=0; i<obj->clip->numplanes; i++) {
          if ((obj->clip->planes[i * 4    ] * hit.x +
               obj->clip->planes[i * 4 + 1] * hit.y +
               obj->clip->planes[i * 4 + 2] * hit.z) > 
               obj->clip->planes[i * 4 + 3]) {
            return; /* hit point was clipped */
          } 
        } 
      }

      ry->maxdist = t;
      ry->intstruct.num=1;
      ry->intstruct.closest.obj = obj;
      ry->intstruct.closest.t = t;
    }
  }
}
