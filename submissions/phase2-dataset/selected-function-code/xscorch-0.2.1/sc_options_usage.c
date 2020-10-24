
   printf("Usage:  %s [options]\n\n", progname);

   printf("--config=file       Load an alternate config file\n");
   printf("--geometry=nxm, -g  Set geometry to width n, height m\n");
   printf("--help, -h          Display this help\n");
   printf("--insanity          If you have to ask ...\n");
   printf("--yields            Display weapon yields\n");
   printf("--sound, -S         Enable music and sound effects\n");
   printf("--nosound, -s       Disable music and sound effects\n");
   printf("--hqmixer           Enable use of high-quality mixer\n");
   printf("--nohqmixer         Disable use of high-quality mixer\n");
   #if USE_NETWORK
      printf("--name=playername   Player name to use in network connection\n");
      printf("--port=number       Network port to use (client or server)\n");
      printf("--client=servername Connect as a client to specified server\n");
      printf("--server            Start a new server\n");
   #endif /* Network? */

   printf("\n");
   return(1);

}
