/* sc_registry_iter_new
   Allocate an sc_registry_iter struct for fast registry set iteration. */

   sc_registry_iter *iter;

   if(registry == NULL) return(NULL);

   iter = (sc_registry_iter *)malloc(sizeof(sc_registry_iter));
   if(iter == NULL) return(NULL);

   /* Prep for the iteration. */
   iter->arg = arg;
   iter->class = class;
   iter->current = NULL;
   iter->direction = direction;
   iter->function = function;
   iter->registry = registry;
   iter->running = true;

   return(iter);

}
