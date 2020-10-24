void sc_weapon_print_yields(const sc_weapon_config *wc) {
/* sc_weapon_print_yields
   Print current weapon yields.
   This is very unoptimized, in favor of stressing things more.
   It can help test/troubleshoot the inventorying system... */

   double price, yield, precision, econ_yield, econ_prec, prec_yield;
   int count = sc_weapon_count(wc, SC_WEAPON_LIMIT_NONE);
   const sc_weapon_info *info;

   /* Header */
   printf("\n   %-20s  %8s  %12s  %12s  %12s  %12s  %12s\n",
          "Weapon Name", "Price", "Yield", "Precision", "Econ Yield", "Econ Prec", "Prec Yield");

   /* Get the first weapon. */
   info = sc_weapon_first(wc, SC_WEAPON_SCAN_FORWARD);

   /* Loop over the weapon space. */
   while(count--) {
      assert(info != NULL);
      if(info->bundle > 0) {
         price      = sc_weapon_statistic(wc, info, NULL, SC_WEAPON_STAT_PRICE);
         yield      = sc_weapon_statistic(wc, info, NULL, SC_WEAPON_STAT_YIELD);
         precision  = sc_weapon_statistic(wc, info, NULL, SC_WEAPON_STAT_PRECISION);
         econ_yield = sc_weapon_statistic(wc, info, NULL, SC_WEAPON_STAT_ECON_YIELD);
         econ_prec  = sc_weapon_statistic(wc, info, NULL, SC_WEAPON_STAT_ECON_PRECISION);
         prec_yield = sc_weapon_statistic(wc, info, NULL, SC_WEAPON_STAT_PRECISION_YIELD);
         printf("   %-20s  %8.0f  %12.0f  %12.9f  %12.0f  %12.9f  %12.0f\n",
                info->name, price, yield, precision, econ_yield, econ_prec, prec_yield);
      }
      /* Get the next weapon. */
      info = sc_weapon_next(wc, info, SC_WEAPON_SCAN_FORWARD);
   }

}
