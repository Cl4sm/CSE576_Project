/* sc_weapon_lookup_by_name
   Tries to find a weapon by roughly the requested name.
   This is much slower than sc_weapon_lookup. */

   sc_registry_iter *iter;
   sc_weapon_test_arg wta;
   sc_weapon_info *info;

   if(wc == NULL || name == NULL) return(NULL);

   /* Set up for iteration. */
   wta.name = name;
   iter = sc_registry_iter_new(wc->registry, wc->registryclass,
                               (flags & SC_WEAPON_SCAN_REVERSE) ? SC_REGISTRY_REVERSE : SC_REGISTRY_FORWARD,
                               _sc_weapon_test_lookup_name, (long)(&wta));
   if(iter == NULL) return(NULL);

   /* Iterate using the fast registry iterators. */
   info = (sc_weapon_info *)sc_registry_iterate(iter);

   /* Clean up. */
   sc_registry_iter_free(&iter);

   /* Verify that the rules in place allow viewing the weapon. */
   if(info != NULL && _sc_weapon_viewable(wc, info, flags))
      return(info);
   else
      return(NULL);

}
