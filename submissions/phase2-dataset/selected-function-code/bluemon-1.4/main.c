{
   DBusMessage* msg;
   DBusMessageIter args;
   DBusConnection* conn;
   char* sigvalue;
   char* upcmd = NULL;
   char* downcmd = NULL;
   char* btid = NULL;
   char* servicename = NULL;
   char s[30];
   time_t t;
   bool protect = false;
   uint32_t uid;

   spurn_children();

   progname = argv[0];
   static struct option long_opts[] = {
      {"help", 0, 0, 'h'},
      {"verbose", 0, 0, 'v'},
      {"version", 0, 0, 'V'},
      {"upcmd", 0, 0, 'u'},
      {"downcmd", 0, 0, 'd'},
      {"btid", 0, 0, 'b'},
      {"protect", 0, 0, 'p'},
   };
   int c;
   int idx = 0;
   while (-1 != (c = getopt_long(argc, argv, "-u:d:b:hVvp",long_opts, &idx))) {
      switch (c) {
          case 'u':
            upcmd = optarg;
            break;
          case 'd':
            downcmd = optarg;
            break;
          case 'b':
            btid = optarg;
            break;
          case 'p':
            protect = true;
            break;
          case 'v':
            verbose = true;
            break;
          case 'V':
            printf("Bluemon client version %s.\n", VERSION);
            break;
          case 'h':
          case ':':
          case '?':
          case 1:
            syntax();
            break;
      }
   }
   if (NULL == upcmd || NULL == downcmd || btid == NULL) syntax();

   conn = dbusConnect();

   // loop listening for signals being emmitted
   while (true) {

      // non blocking read of the next available message
      dbus_connection_read_write(conn, -1);
      while (NULL != (msg = dbus_connection_pop_message(conn))) {

         message(true, "Got Message: %s.%s\n", dbus_message_get_interface(msg),
               dbus_message_get_member(msg));

         // check if the message is a signal from the correct interface and with the correct name
         // and from the correct source on the bus
         if (dbus_message_is_signal(msg, "cx.ath.matthew.bluemon.ProximitySignal", "Connect")) {
            if (!protect || 0 == (uid = getConnectionUnixUser(dbus_message_get_sender(msg), conn))) {

               // read the parameters
               if (!dbus_message_iter_init(msg, &args))
                  message(true, "Message Has No Parameters\n");
               else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
                  message(true, "Argument is not string!\n"); 
               else {
                  dbus_message_iter_get_basic(&args, &sigvalue);
                  time(&t);
                  strftime(s, 30, "%H:%M", localtime(&t));
                  message(true, "[%s] Got Connect Signal for %s\n", s, sigvalue);
                  if (0 == strncasecmp(btid, sigvalue, strlen(btid)))
                     fork_and_run(upcmd);
               }
            }
            else
               message(true, "Got signal from %d, not root\n", uid);
         }
         else if (dbus_message_is_signal(msg, "cx.ath.matthew.bluemon.ProximitySignal", "Disconnect")) {
            if (!protect || 0 == (uid = getConnectionUnixUser(dbus_message_get_sender(msg), conn))) {

               // read the parameters
               if (!dbus_message_iter_init(msg, &args))
                  message(true, "Message Has No Parameters\n");
               else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
                  message(true, "Argument is not string!\n"); 
               else {
                  dbus_message_iter_get_basic(&args, &sigvalue);
                  time(&t);
                  strftime(s, 30, "%H:%M", localtime(&t));
                  message(true, "[%s] Got Disconnect Signal for %s\n", s, sigvalue);
                  if (0 == strncasecmp(btid, sigvalue, strlen(btid)))
                     fork_and_run(downcmd);
               }
            }
            else
               message(true, "Got signal from %d, not root\n", uid);
         }
         else
            message(true, "Spurious message from %s\n",  dbus_message_get_sender(msg));

         // free the message
         dbus_message_unref(msg);
      }
   }
   // close the connection
   dbus_connection_close(conn);

}
