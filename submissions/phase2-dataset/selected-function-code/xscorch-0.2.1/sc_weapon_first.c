sc_weapon_info *sc_weapon_first(const sc_weapon_config *wc, int flags) {
/* sc_weapon_first
   Return the first/last viable weapon in the weapon config space. */

   sc_weapon_test_arg wta;
   sc_weapon_info *info;

   if(wc == NULL) return(NULL);

   wta.wc    = wc;
   wta.flags = flags;

   /* Try and find the first/last weapon that meets our criteria. */
   info = sc_registry_find_first(wc->registry, wc->registryclass,
                                 (flags & SC_WEAPON_SCAN_REVERSE) ? SC_REGISTRY_REVERSE : SC_REGISTRY_FORWARD,
                                 _sc_weapon_test_viewable, (long)(&wta));

   return(info);

}
