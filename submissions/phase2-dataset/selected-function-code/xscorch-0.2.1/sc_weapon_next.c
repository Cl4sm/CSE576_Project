/* sc_weapon_next
   Advance to the next/prev weapon in the list (with wrapping).  */

   sc_weapon_test_arg wta;
   sc_weapon_info *nxtnfo;

   if(wc == NULL || info == NULL) return(NULL);

   /* Set up the flags and args for the mission. */
   wta.wc    = wc;
   wta.flags = flags;

   /* Try and find the first/last weapon that meets our criteria. */
   nxtnfo = sc_registry_find_next(wc->registry, wc->registryclass, info->ident,
                                  (flags & SC_WEAPON_SCAN_REVERSE) ? SC_REGISTRY_REVERSE : SC_REGISTRY_FORWARD,
                                  _sc_weapon_test_viewable, (long)(&wta));

   /* In case we iterated off the end of the list, wrap to the beginning. */
   if(nxtnfo == NULL)
      nxtnfo = sc_weapon_first(wc, flags);

   return(nxtnfo);

}
