int sc_accessory_count(const sc_accessory_config *ac, int flags) {
/* sc_accessory_count
   Counts the number of accessories that have been registered with the game's data registry.
   This is optimized but of course if you're calling it a lot you should cache the data.
   The only exception is that the count may change if a new accessory list file is appended. */

   sc_registry_iter *iter;
   sc_accessory_test_arg ata;

   if(ac == NULL) return(0);

   /* Set up for iteration. */
   ata.counter = 0;
   ata.flags   = flags;
   ata.ac      = ac;
   iter = sc_registry_iter_new(ac->registry, ac->registryclass, SC_REGISTRY_FORWARD,
                               _sc_accessory_test_count, (long)(&ata));
   if(iter == NULL) return(0);

   /* Iterate using the fast registry iterators. */
   sc_registry_iterate(iter);

   /* Clean up. */
   sc_registry_iter_free(&iter);

   return(ata.counter);

}
