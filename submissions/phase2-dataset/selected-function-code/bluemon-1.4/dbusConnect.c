DBusConnection* dbusConnect()
{
   DBusConnection* conn;
   DBusError err;
   int ret;

   // initialise the errors
   dbus_error_init(&err);
   
   // connect to the bus and check for errors
   conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
   if (dbus_error_is_set(&err)) { 
      message(false, "Connection Error (%s)\n", err.message);
      dbus_error_free(&err); 
   }
   if (NULL == conn) { 
      exit(1);
   }
   
   // add a rule for which messages we want to see
   dbus_bus_add_match(conn, "type='signal',interface='cx.ath.matthew.bluemon.ProximitySignal'", &err); // see signals from the given interface
   dbus_connection_flush(conn);
   if (dbus_error_is_set(&err)) { 
      message(false, "Match Error (%s)\n", err.message);
      exit(1); 
   }
   return conn; 
}
