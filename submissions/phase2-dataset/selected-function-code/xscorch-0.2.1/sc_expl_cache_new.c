/* sc_expl_cache_new
   Creates a new explosion with these characteristics, and returns its cache
   ID.  Note, if an explosion of these characteristics already exists in the
   cache, then its cache ID will be returned, and no new explosion will be
   created.  */

   sc_window_gtk *w = (sc_window_gtk *)w_;   /* Window structure */
   sc_expl_cache_gtk *cache =  w->explcache; /* Cache structure */
   sc_expl_cache_entry_gtk *centry; /* An entry in the cache. */
   int cacheid;      /* Cache ID of a matching explosion */
   int size;         /* Size (wid/hei) of the pixmap to create. */

   /* Try to find this explosion in the cache. */      
   cacheid = _sc_expl_cache_lookup_gtk(w, radius, type);
   if(cacheid < 0) {
      /* No matching explosion found; overwrite the oldest
         entry in the cache with our new explosion.  */
      /* TEMP BUG:  We really shouldn't overwrite the OLDEST
         entry; we'd get better performance if we overwrite
         the LEAST-RECENTLY-USED entry.  Need to fix this... */
      --cache->headptr;
      if(cache->headptr < 0) cache->headptr = SC_EXPL_CACHE_SIZE - 1;
      cacheid = cache->headptr;

      /* Get the entry for the explosion we are overwriting */
      centry = &cache->cache[cacheid];
      if(cache->cachesize < SC_EXPL_CACHE_SIZE) {
         /* Cache wasn't full; this entry is vacant. */
         ++cache->cachesize;
      } else {
         /* Cache was full; need to release the old pixmaps */
         g_object_unref(centry->pixmap);
      } /* Was cache already full? */

      /* Create the new pixmap and bitmap, and set characteristics */
      size = radius + radius + 1;   
      centry->pixmap = gdk_pixmap_new(w->app->window, size, size, -1);
      centry->radius = radius;
      centry->type   = type;

      /* Draw an explosion into the new pixmap/bitmap. */
      _sc_expl_cache_annihilate_gtk(w, centry->pixmap, radius, type);
   } /* Did explosion already exist in cache? */

   /* Return the cache ID of our new explosion */   
   return(cacheid);

}
