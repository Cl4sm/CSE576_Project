void sendsignal(bool inrange, btdev_t* btdev)
{
   DBusMessage* msg;
   DBusMessageIter it;
   dbus_uint32_t serial;
   if (inrange)
      msg = dbus_message_new_signal("/cx/ath/matthew/bluemon/Bluemon", "cx.ath.matthew.bluemon.ProximitySignal", "Connect");
   else
      msg = dbus_message_new_signal("/cx/ath/matthew/bluemon/Bluemon", "cx.ath.matthew.bluemon.ProximitySignal", "Disconnect");
   if (NULL == msg) { message(true, "DBUS Message Null, ALIEN INVASION\n"); exit(1); }
   dbus_message_iter_init_append(msg, &it);
   if (!dbus_message_iter_append_basic(&it, DBUS_TYPE_STRING, &(btdev->btid))) {
      message(true, "Out Of Memory!");
      exit(1);
   }
   if (!dbus_connection_send(conn, msg, &serial)) {
      message(true, "Out Of Memory!");
      exit(1);
   }

   //dbus_connection_flush(conn);
   dbus_message_unref(msg);
   message(true, "Signal sent (%s, %s)\n", inrange?"true":"false", btdev->btid);
}
