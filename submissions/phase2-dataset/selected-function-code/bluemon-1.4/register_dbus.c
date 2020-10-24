{
   DBusError err;
   DBusConnection* conn;
   int ret;

   dbus_error_init(&err);

   conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
   if (dbus_error_is_set(&err)) { 
      message(true, "DBUS Connection Error (%s)\n", err.message); 
      dbus_error_free(&err); 
   }
   if (NULL == conn) { 
      exit(1);
   }

   ret = dbus_bus_request_name(conn, "cx.ath.matthew.bluemon.server", 
                               DBUS_NAME_FLAG_REPLACE_EXISTING, 
                               &err);
   if (dbus_error_is_set(&err)) {
      message(true, "DBUS Name Error (%s)\n", err.message); 
      dbus_error_free(&err);
   }
   if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) { 
      exit(1);
   }
   return conn;
}
