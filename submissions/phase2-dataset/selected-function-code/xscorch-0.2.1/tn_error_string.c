/*
 * tn_error_string
 * Generate a string describing the connection state.
 */

   int state;
   const char *ret;

   static const char *strings[11] = {
      /* Single-bit errors. */
      "Uninitialized",
      "Connected, no error",
      "Connection lost",
      "Malloc failed",
      "Packet misalignment detected",
      "Packet received out of order",
      "Select failed",
      "Unknown error",

      /* Other. */
      "Multiple errors detected",
      "Invalid argument to tn_error_string",
       NULL
   };

   /* Early return. */
   if(tnc == NULL) return(strings[9]);

   state = tnc->state;

   /* Figure out what's up and tell the user. */
   switch(state) {
      case TN_STATE_DEFAULT:
         ret = strings[0];
         break;
      case TN_STATE_CONNECTED:
         ret = strings[1];
         break;
      case TN_STATE_ERR_CONNLOST:
         ret = strings[2];
         break;
      case TN_STATE_ERR_MALLOC:
         ret = strings[3];
         break;
      case TN_STATE_ERR_MISALIGN:
         ret = strings[4];
         break;
      case TN_STATE_ERR_MISORDER:
         ret = strings[5];
         break;
      case TN_STATE_ERR_SELECT:
         ret = strings[6];
         break;
      case TN_STATE_ERR_UNKNOWN:
         ret = strings[7];
         break;
      default:
         ret = strings[8];
         break;
   }

   return(ret);

}
