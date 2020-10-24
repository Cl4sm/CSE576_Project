void sc_weapon_info_line(const sc_weapon_config *wc, const sc_weapon_info *info, char *buffer, int buflen) {
/* sc_weapon_info_line
   Create a line of information about the weapon. */

   int moving;

   assert(wc != NULL);
   assert(buflen >= 0);

   if(buffer == NULL || buflen == 0)
      return;

   if(info == NULL) {
      buffer[0] = '\0';
      return;
   }

   /* Clear the buffer. */
   memset(buffer, 0, buflen * sizeof(char));
   /* Terminating NULL character. */
   --buflen;

   /* Add the name to the buffer. */
   sbprintf(buffer, buflen, "%s:", info->name);
   moving = strlenn(info->name) + 1;
   buffer += moving;
   buflen -= moving;

   /* Add spaces out to the end of the name area. */
   while(++moving < 20 && --buflen > 0)
      *(buffer++) = ' ';

   /* Display the weapon's arms level. */
   sbprintf(buffer, buflen, "arms = %1i, ", info->armslevel);
   moving = 10;
   buffer += moving;
   buflen -= moving;

   /* Display the weapon's yield. */
   moving = sc_weapon_statistic(wc, info, NULL, SC_WEAPON_STAT_YIELD) / 1000;
   sbprintf(buffer, buflen, "yield = %-8i", moving);
   moving = 8 + (moving ? (int)log10(moving) : 1);
   buffer += moving;
   buflen -= moving;

   /* Add the comma. */
   if(buflen-- > 0)
      *(buffer++) = ',';

   /* Add spaces out to the end of the yield area, plus two. */
   while(++moving < 18 && --buflen > 0)
      *(buffer++) = ' ';

   /* Write out some weapon info flags. */
   sbprintf(buffer, buflen, "%7s %8s %5s %6s %7s",
            SC_WEAPON_IS_PHOENIX(info) ? "complex" : "",
            SC_WEAPON_IS_INFINITE(info) ? "infinite" : "",
            SC_WEAPON_IS_SMOKING(info) ? "smoke" : "",
            SC_WEAPON_IS_TRIPLE(info) ? "triple" : "",
            info->useless ? "useless" : "");

}
