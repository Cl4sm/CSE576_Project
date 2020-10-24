/* sc_registry_iterate
   Find and return the next item in the set, if any. */

   if(iter == NULL) return(NULL);

   /* Make sure we're still iterating happily along. */
   if(!iter->running)
      return(NULL);

   /* Perform the search for the next iteration. */
   iter->current = _sc_registry_internal_iter(iter->registry->hashlist, iter->current,
                                              iter->class, iter->direction,
                                              iter->function, iter->arg);

   if(iter->current == NULL) {
      /* We've reached the end of the set. */
      iter->running = false;
      return(NULL);
   } else {
      return(iter->current->data);
   }

}
