/* sc_registry_new
   Allocate and initialize a new registry. */

   sc_registry *registry;

   /* Allocate the registry. */
   registry = (sc_registry *)malloc(sizeof(sc_registry));
   if(registry == NULL)
      return(NULL);

   /* Allocate the storage hashlist. */
   registry->hashlist = shashlist_new(SC_REGISTRY_SIZE);
   if(registry->hashlist == NULL) {
      free(registry);
      return(NULL);
   }

   /* Start numbering custom classes at the first available integer. */
   registry->nextclass = SHASHLIST_FIRST_CLASS;

   /* Start numbering valid registry keys at 0 (-1 means bad key). */
   registry->nextkey = 0;

   return(registry);

}
