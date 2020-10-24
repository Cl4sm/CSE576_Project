uint32_t getConnectionUnixUser(const char* service, DBusConnection* conn)
{
   DBusMessage* msg;
   DBusMessageIter args;
   DBusPendingCall* pending;
   uint32_t uid;
   char* errmsg;

   if (NULL == service) return -1;
   msg = dbus_message_new_method_call("org.freedesktop.DBus", // target for the method call
         "/org/freedesktop/DBus", // object to call on
         "org.freedesktop.DBus", // interface to call on
         "GetConnectionUnixUser"); // method name
   if (NULL == msg) { 
      message(false, "Message Null\n");
      exit(1);
   }

   // append arguments
   dbus_message_iter_init_append(msg, &args);
   if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &service)) { 
      message(false, "Out Of Memory!\n"); 
      exit(1);
   }

   // send message and get a handle for a reply
   if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
      message(false, "Out Of Memory!\n"); 
      exit(1);
   }
   if (NULL == pending) { 
      message(false, "Pending Call Null\n"); 
      exit(1); 
   }
   dbus_connection_flush(conn);

   // free message
   dbus_message_unref(msg);

   // block until we recieve a reply
   dbus_pending_call_block(pending);

   // get the reply message
   msg = dbus_pending_call_steal_reply(pending);
   if (NULL == msg) {
      message(false, "Reply Null\n"); 
      exit(1); 
   }
   // free the pending message handle
   dbus_pending_call_unref(pending);

   if (DBUS_MESSAGE_TYPE_ERROR == dbus_message_get_type(msg)) {
      message(false, "Failed to check owner\n");
   }
   // read the parameters
   else if (!dbus_message_iter_init(msg, &args))
      message(false, "Message has no arguments!\n"); 
   else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) 
      message(false, "Argument is not uint32 (is %c)!\n",  dbus_message_iter_get_arg_type(&args)); 
    else {
      dbus_message_iter_get_basic(&args, &uid);
      dbus_message_unref(msg);   
      return uid;
   }

   dbus_message_unref(msg);   
   return -1;
}
