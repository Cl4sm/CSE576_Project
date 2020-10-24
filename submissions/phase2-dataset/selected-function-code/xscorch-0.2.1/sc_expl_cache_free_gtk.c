void sc_expl_cache_free_gtk(sc_expl_cache_gtk **cache) {
/* sc_expl_cache_free_gtk
   Release the cache, and associated data.  */
   
   int i;            /* Iterator variable */
   
   /* Sanity check. */
   if(cache == NULL || *cache == NULL) return;
   
   /* Release pixmaps and bitmaps in cache */
   for(i = 0; i < (*cache)->cachesize; ++i) {
      g_object_unref((*cache)->cache[(*cache)->headptr].pixmap);
      ++(*cache)->headptr;
      if((*cache)->headptr >= SC_EXPL_CACHE_SIZE) {
         (*cache)->headptr = 0;
      }
   } /* Releasing all pixmaps, bitmaps... */
   
   /* Release the GC */
   g_object_unref((*cache)->bitmapgc);
   g_object_unref((*cache)->fakebitmap);
   
   /* Release the cache memory */
   free(*cache);
   *cache = NULL;
   
}
