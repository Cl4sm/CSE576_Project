/* sc_expl_cache_new_gtk
   Creates a new explosion cache.  */
   
   sc_expl_cache_gtk *cache;  /* Newly allocated cache */
   
   /* Create the cache */
   cache = (sc_expl_cache_gtk *)malloc(sizeof(sc_expl_cache_gtk));
   if(cache == NULL) return(NULL);
   
   /* Initialise the cache */
   cache->fakebitmap = gdk_pixmap_new(NULL, 1, 1, 1);
   cache->bitmapgc = gdk_gc_new(cache->fakebitmap);
   cache->cachesize = 0;
   cache->headptr = 0;
   
   /* Return the cache */
   return(cache);
   
}
