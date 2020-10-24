report_failure(path, code)
char *path;
int code;
{
  switch(code) {
    case -1: fprintf(stderr, "%s:\nUnknown image format !!\n", path);
             break;

    case 0:  break;

    case 1:  fprintf(stderr, "Cannot read file %s !!\n", path); 
             break;

    case 2:  fprintf(stderr, "File %s has corrupted data!!\n", path);
             break;

    case 3:  fprintf(stderr, "Cannot decode format specification of %s !!\n", 
             path);
             break;

    case 4:  fprintf(stderr, 
                "Image creation failed (memory alloc. problem?) !!\n");
             break;

    case 5:  fprintf(stderr, "Header of file %s corrupted !!\n", path);
             break;

    case 6:  fprintf(stderr, "Color allocation failed !!\n");
             break;

    case 7:  fprintf(stderr, "Trying instead default  %s\n", path);
             break;

    default:
             fprintf(stderr, "Unknown error in  %s !!\n", path);
             break;
  }
}
