/* sc_accessory_info_line
   Create a line of information about the accessory. */

   int moving;

   assert(ac != NULL);
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
   moving = strblenn(info->name, SC_INVENTORY_MAX_NAME_LEN) + 1;
   buffer += moving;
   buflen -= moving;

   /* Add spaces out to the end of the name area. */
   while(++moving < 20 && --buflen > 0)
      *(buffer++) = ' ';

   /* Display the accessory's arms level. */
   sbprintf(buffer, buflen, "arms = %1i, ", info->armslevel);
   moving = 10;
   buffer += moving;
   buflen -= moving;

   /* We display much different stuff for shields than for other accessories. */
   if(SC_ACCESSORY_IS_SHIELD(info)) {
      /* Display the shield level. */
      moving = info->shield;
      sbprintf(buffer, buflen, "shield = %-5i", moving);
      moving = 9 + (moving ? (int)log10(moving) : 1);
      buffer += moving;
      buflen -= moving;

      /* Add the comma. */
      if(buflen-- > 0)
         *(buffer++) = ',';

      /* Add spaces out to the end of the yield area, plus two. */
      while(++moving < 16 && --buflen > 0)
         *(buffer++) = ' ';

      /* Write out the shield short name. */
      sbprintf(buffer, buflen, "%3i%1c ", info->shield / 100, SC_ACCESSORY_SHIELD_CHAR(info));
      moving = 5;
      buffer += moving;
      buflen -= moving;

      /* And the shield type, of course. */
      sbprintf(buffer, buflen, "%s",
               SC_ACCESSORY_SHIELD_IS_FORCE(info) ? "force" :
               (SC_ACCESSORY_SHIELD_IS_MAGNETIC(info) ? "magnetic" :
                (SC_ACCESSORY_SHIELD_IS_STANDARD(info) ? "standard" : "unknown")));
      moving = 8;
      buffer += moving;
      buflen -= moving;

      /* If magnetic, also give repulsion. */
      if(SC_ACCESSORY_SHIELD_IS_MAGNETIC(info))
         sbprintf(buffer, buflen, " (repulsion = %i)", info->repulsion);
   } else {
      /* Write out some accessory info flags. */
      sbprintf(buffer, buflen, " %10s %8s %9s %7s",
               SC_ACCESSORY_IS_CONSUMABLE(info) ? "consumable" : "",
               SC_ACCESSORY_IS_INFINITE(info) ? "infinite" : "",
               SC_ACCESSORY_IS_PERMANENT(info) ? "permanent" : "",
               info->useless ? "useless" : "");
   }

}
