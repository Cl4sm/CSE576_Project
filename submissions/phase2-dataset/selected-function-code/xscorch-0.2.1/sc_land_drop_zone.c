/* sc_land_drop
   Drop the land in the columns x1 to x2 (inclusive).  If land still needs
   to be dropped after this call, then nonzero is returned.  X is a virtual
   coordinate.  */

   bool needrecurse; /* Nonzero if we need to recurse */
   int cx;           /* Current column we are operating on */

   /* Sanity check. */
   if(c == NULL || l == NULL) return(false);

   needrecurse = false;
   for(cx = x1; cx <= x2; ++cx) {
      /* CX may be virtual when given to drop_column. */
      needrecurse = _sc_land_drop_column(c, l, cx) || needrecurse;
   } /* End interval loop */

   /* Will user need to call this method again? */
   return(needrecurse);

}
