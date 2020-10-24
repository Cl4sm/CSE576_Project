/* sc_phoenix_verify
   Recursively scan the weapon's info structure and check for fatal errors. */

   const sc_weapon_info *child;
   slcache *temp = cache;
   slcache current;
   int count = 1;
   int level = 0;

   /* Create cache entry for this level. */
   prev->next = &current;
   current.id = SC_WEAPON_TYPE(info);
   current.next = NULL;

   /* DETONATE types aren't *really* phoenix weapons, per se... */
   if(SC_WEAPON_IS_PHOENIX(info) && !SC_PHOENIX_IS_DETONATE(info)) {
      /* Find out what we're expecting as a child. */
      child = sc_weapon_lookup(wc, SC_PHOENIX_CHILD_TYPE(info), SC_WEAPON_LIMIT_NONE);
      /* A non-extant child will really throw a sabo into the machinery. */
      if(child == NULL) return(0);
      /* If we are our own child then we're staring at the face of infinity... */
      if(SC_WEAPON_TYPE(info) == SC_WEAPON_TYPE(child)) return(0);
      /* Scan the weapon's ancestors for loop conditions. */
      while(temp != prev->next) {
         /* If the current weapon is among its ancestors, kill it. */
         if(SC_WEAPON_TYPE(info) == temp->id) return(0);
         temp = temp->next;
      }
      /* Find out what the levels under our child are. */
      level = _sc_phoenix_safe_verify(wc, child, cache, &current);
      /* If our child is invalid, then we are. */
      if(level == 0) return(0);
      /* Add up the potential levels under each type we have. */
      if(SC_PHOENIX_IS_CHAIN(info))   count += level;
      if(SC_PHOENIX_IS_SPIDER(info))  count += level;
      if(SC_PHOENIX_IS_SCATTER(info)) count += level;
   }

   /* A non-phoenix or DETONATE weapon is by definition acceptable.
      Besides, that's the stopping condition... */

   /* Release this level of the cache. */
   prev->next = NULL;

   /* Return count of this and sub levels. */
   return(count);

}
