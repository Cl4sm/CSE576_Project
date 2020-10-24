static void usage(char *app)
{
  int i;

  printf("since displays the data appended to files since the last time since was run\n\n");

  printf("Usage: %s [option ...] file ...\n", app);

  printf("\nOptions\n");
  printf(" -a        update state file atomically\n");
  printf(" -d int    set the interval when following files\n");
  printf(" -e        print header lines to standard error\n");
  printf(" -f        follow files, periodically check if more data has been appended\n");
  printf(" -h        this help\n");
  printf(" -l        lax mode, do not fail if some files are inaccessible\n");
  printf(" -m        do not use mmap() to access files, use read()\n");
  printf(" -n        do not update since state file\n");
  printf(" -q        reduce verbosity to nothing\n");
  printf(" -s file   specify the state file, overriding SINCE variable and home directory\n");
  printf(" -v        increase verbosity, can be given multiple times\n");
  printf(" -x        ignore files with compressed extensions:");
  for(i = 0; ignore_suffix[i]; i++){
    printf(" %s", ignore_suffix[i]);
  }
  printf("\n");
  printf(" -z        discard initial output\n");
#ifdef VERSION
  printf(" -V        print version information\n");
#endif

  printf("\nExample\n");
  printf(" $ since -lz /var/log/*\n");
  printf(" $ logger foobar\n");
  printf(" $ since -lx /var/log/*\n");
}