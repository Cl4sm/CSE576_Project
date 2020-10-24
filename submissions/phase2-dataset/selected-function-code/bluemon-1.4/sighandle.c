void sighandle(int signum)
{
   btdev_t* btdev;
   switch (signum) {
      case SIGINT:
      case SIGTERM:
         btdev = testdev(args.btdevroot, opts.btdevroot, "No Devices Specified");
         while (NULL != btdev) {
            if (btdev->connected) {
               // disconnect bluetooth
               btdisconnect(btdev);
               message(true, "Disconnected %s\n", btdev->btid);
            }
            if (NULL != conn) 
               sendsignal(false, btdev);
            hci_close_dev(btdev->dd);
            btdev_t* temp = btdev->next;
            free(btdev);
            btdev = temp;
         }
         message(false, "Caught SIGTERM, announcing our demise\n");
         if (NULL != conn) 
            dbus_connection_unref(conn);
         exit(0);
      case SIGHUP:
         readoptions(args, &opts);
         break;
   }
}
