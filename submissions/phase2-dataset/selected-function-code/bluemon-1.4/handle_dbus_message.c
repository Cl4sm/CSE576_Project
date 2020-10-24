void handle_dbus_message(DBusMessage* msg, int threshold)
{
   DBusMessage* reply;
   DBusMessageIter it;
   dbus_uint32_t serial, level;
   dbus_bool_t stat;
   char* address;

   if (!dbus_message_iter_init(msg, &it)) {
      message(true, "Bad parameter count for Status message");
      return;      
   }
   if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&it)) {
      message(true, "Bad parameter type (%d) for Status message", dbus_message_iter_get_arg_type(&it));
      return;      
   }
   dbus_message_iter_get_basic(&it, &address);
   message(true, "Handling dbus status query for %d\n", address);
   reply = dbus_message_new_method_return(msg);
   if (NULL == reply) {
      message(true, "Reply is NULL!! You have been eaten by a GRUE!");
      return;      
   }
   dbus_message_iter_init_append(reply, &it);
   btdev_t* btdev = testdev(args.btdevroot, opts.btdevroot, "No Devices Specified");
   for (; NULL != btdev; btdev = btdev->next) {
      if (0 == strncmp("", address, 1) || 0 == strncasecmp(btdev->btid, address, strlen(btdev->btid))) {
         if (!dbus_message_iter_append_basic(&it, DBUS_TYPE_STRING, &(btdev->btid))) {
            message(true, "Out Of Memory! Lock me in the cellar and feed me pins, PINS!!");
            dbus_message_unref(reply);
            return;      
         }
         if (btdev->track >= threshold) {
            stat = TRUE;
            level = btdev->track;
         } else {
            stat = FALSE;
            level = -1;
         }
         break;
      } else {
         //address = "";
         stat = FALSE;
         level = -1;
         dbus_message_iter_append_basic(&it, DBUS_TYPE_STRING, &address);
      }
   }
   if (!dbus_message_iter_append_basic(&it, DBUS_TYPE_BOOLEAN, &stat)){
      message(true, "Out Of Memory! The bells! the bells!");
      dbus_message_unref(reply);
      return;      
   }

   if (!dbus_message_iter_append_basic(&it, DBUS_TYPE_UINT32, &level)){
      message(true, "Out Of Memory! Every night the steam part of my brain and feed it to their mum!");
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
