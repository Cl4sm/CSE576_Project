sc_registry_iter *sc_registry_iter_new(const sc_registry *registry, int class, sc_registry_direction direction, sc_registry_test_func function, long arg) {
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
