int sc_weapon_count(const sc_weapon_config *wc, int flags) {
/* sc_weapon_count
   Counts the number of weapons that have been registered with the game's data registry.
   This is optimized but of course if you're calling it a lot you should cache the data.
   The only exception is that the count may change if a new weapon list file is appended. */

   sc_registry_iter *iter;
   sc_weapon_test_arg wta;

   if(wc == NULL) return(0);

   /* Set up for iteration. */
   wta.counter = 0;
   wta.flags   = flags;
   wta.wc      = wc;
   iter = sc_registry_iter_new(wc->registry, wc->registryclass, SC_REGISTRY_FORWARD,
                               _sc_weapon_test_count, (long)(&wta));
   if(iter == NULL) return(0);

   /* Iterate using the fast registry iterators. */
   sc_registry_iterate(iter);

   /* Clean up. */
   sc_registry_iter_free(&iter);

   return(wta.counter);

}
