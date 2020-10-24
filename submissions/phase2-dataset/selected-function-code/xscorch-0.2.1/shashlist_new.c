/* shashlist_new
   Create and initialize a new shashlist. */

   shashlist *list;
   int roundup = 0;

   list = (shashlist *)malloc(sizeof(shashlist));
   if(list == NULL)
      return(NULL);

   /* The current implementation requires a hash size in 2^n. */
   list->hashbits = 0;
   while(hashsize > 1) {
      ++list->hashbits;
      if(hashsize % 2)
         ++roundup;
      hashsize = hashsize >> 1;
   }
   if(roundup) ++list->hashbits;

   /* SHASH_MAX_BITS comes from shash.h */
   if(list->hashbits > SHASH_MAX_BITS) list->hashbits = SHASH_MAX_BITS;

   /* Allocate the (possibly large) array of item pointers for the hash. */
   list->hash = (shashlist_item **)malloc((1 << list->hashbits) * sizeof(shashlist_item *));
   if(list->hash == NULL) {
      free(list);
      return(NULL);
   }

   list->head = NULL;
   list->tail = NULL;

   return(list);

}
