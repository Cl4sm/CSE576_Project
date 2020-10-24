{
   DBusMessage* reply;
   DBusMessageIter it;
   dbus_uint32_t serial;
   char* xml = DBUS_INTROSPECTION_DATA;

   message(true, "Handling dbus introspection query\n");
   reply = dbus_message_new_method_return(msg);
   if (NULL == reply) {
      message(true, "Reply is NULL!! You have been eaten by a GRUE!");
      return;      
   }
   dbus_message_iter_init_append(reply, &it);
   if (!dbus_message_iter_append_basic(&it, DBUS_TYPE_STRING, &xml)){
      message(true, "Out Of Memory! The bells! the bells!");
      dbus_message_unref(reply);
      return;      
   }

   if (!dbus_connection_send(conn, reply, &serial)){
      message(true, "Out Of Memory! Universe error, redo from start!");
      dbus_message_unref(reply);
      return;      
   }
   dbus_connection_flush(conn);
   dbus_message_unref(reply);
}
