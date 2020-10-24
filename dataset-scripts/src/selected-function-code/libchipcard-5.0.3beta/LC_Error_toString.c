const char *LC_Error_toString(uint32_t err) {
  switch (err) {
  case LC_ERROR_NONE:                   return "none";
  case LC_ERROR_GENERIC:                return "generic";
  case LC_ERROR_INVALID:                return "Invalid argument";
  case LC_ERROR_CARD_REMOVED:           return "Card removed";
  case LC_ERROR_CARD_NOT_OWNED:         return "Card not owned";
  case LC_ERROR_NOT_SUPPORTED:          return "Not supported";
  case LC_ERROR_SETUP:                  return "Setup error";
  case LC_ERROR_NO_DATA:                return "No data";
  case LC_ERROR_LOCKED_BY_OTHER:        return "Locked by other";
  case LC_ERROR_NOT_LOCKED:             return "Not locked";

  case LC_ERROR_BAD_RESPONSE:           return "Bad response";
  case LC_ERROR_NO_SLOTS_CONNECTED:     return "No slots connected";
  case LC_ERROR_NO_SLOTS_DISCONNECTED:  return "No slots disconnected";
  case LC_ERROR_NO_SLOTS_AVAILABLE:     return "No slots available";
  case LC_ERROR_BAD_PIN:                return "Bad pin";
  case LC_ERROR_USER_ABORTED:           return "User aborted";
  case LC_ERROR_CARD_DESTROYED:         return "Card destroyed";
  default:                              return "Unknown error code";
  }
}