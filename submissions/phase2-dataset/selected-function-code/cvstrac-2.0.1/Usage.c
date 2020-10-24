static void Usage(const char *argv0, const char *argvN){
  fprintf(stderr,"%s: Illegal argument \"%s\"\n",argv0,argvN);
  fprintf(stderr,"Usage: %s [options] filename...\n"
    "Options:\n"
    "  -h          Generate a single .h to standard output.\n"
    "  -H          Like -h, but only output EXPORT declarations.\n"
    "  -v          (verbose) Write status information to the screen.\n"
    "  -doc        Generate no header files.  Instead, output information\n"
    "              that can be used by an automatic program documentation\n"
    "              and cross-reference generator.\n"
    "  -local      Generate prototypes for \"static\" functions and\n"
    "              procedures.\n"
    "  -f FILE     Read additional command-line arguments from the file named\n"
    "              \"FILE\".\n"
#ifdef DEBUG
    "  -! MASK     Set the debugging mask to the number \"MASK\".\n"
#endif
    "  --          Treat all subsequent comment-line parameters as filenames,\n"
    "              even if they begin with \"-\".\n",
    argv0
  );
}