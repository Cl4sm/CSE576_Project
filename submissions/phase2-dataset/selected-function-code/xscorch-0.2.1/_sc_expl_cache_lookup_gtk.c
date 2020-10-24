static int _sc_expl_cache_lookup_gtk(sc_window_gtk *w, int radius, sc_explosion_type type) {
/* sc_expl_cache_lookup_gtk
   Lookup an entry in the cache matching this description.
   If no such entry is found, -1 is returned.  */

   sc_expl_cache_gtk *cache = w->explcache;  /* Cache */
   int size;         /* Size of cache */
   int ptr;          /* Current ptr into cache */

   /* Determine size and starting pointer */   
   size = cache->cachesize;
   ptr = cache->headptr;
   
   /* Try to find an explosion matching these characteristics */
   while(size > 0) {
      /* Does this explosion match? */
      if(cache->cache[ptr].radius == radius && cache->cache[ptr].type == type) {
         /* Match found; return its cacheid */
         return(ptr);
      } /* Found a match? */
      
      /* Not a match; advance to next entry in the cache. */
      ++ptr;
      if(ptr >= SC_EXPL_CACHE_SIZE) ptr = 0;
      --size;
   } /* Searching for a match... */

   /* No matching explosion was found */
   return(-1);

}
