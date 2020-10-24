  if (t > EPSILON) {
    /* if we hit something before maxdist update maxdist */
    if (t < ry->maxdist) {
      /* if this object doesn't cast a shadow, and we aren't  */
      /* limiting the number of transparent surfaces to less  */
      /* than 5, then modulate the light by its opacity value */
      if (!(obj->tex->flags & RT_TEXTURE_SHADOWCAST)) {
        if (ry->scene->shadowfilter)
          ry->intstruct.shadowfilter *= (1.0 - obj->tex->opacity);
        return;
      }

      ry->maxdist = t;
      ry->intstruct.num=1;

      /* if we hit *anything* before maxdist, and we're firing a */
      /* shadow ray, then we are finished ray tracing the shadow */
      ry->flags |= RT_RAY_FINISHED;
    }
  }
}
