{
   package_t* package;
   list_node* node;
   int got_one = 0;

   DEBUG(stderr, "using package `%s'...\n", name);
      
   for (node = head(the_packages) ; node ; node = next(node))
   {
      package = (package_t*) get_value(node);

      if (package_matches(package, name, the_host_info.machine,
                          the_host_info.sysname, the_host_info.release,
                          the_host_info.nodename, the_shell))
      {
         add_package(package);
         got_one = 1;
      }
   }

   if ((!silent) && (!got_one))
      fprintf(stderr,
              "warning: no match for package `%s' on this host.\n",
              name);
}
