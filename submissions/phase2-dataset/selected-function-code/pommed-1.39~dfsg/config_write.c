int
config_write(void)
{
  FILE *fp;

  fp = fopen(conffile, "w");
  if (fp == NULL)
    {
      fprintf(stderr, "Could not write to config file: %s\n", strerror(errno));

      return -1;
    }

  fprintf(fp, "# gpomme config file\n");
  fprintf(fp, "#  - theme : name of the theme to use\n");
  fprintf(fp, "#  - timeout : time before the window hides\n\n");

  cfg_print(cfg, fp);

  fclose(fp);

  return 0;
}