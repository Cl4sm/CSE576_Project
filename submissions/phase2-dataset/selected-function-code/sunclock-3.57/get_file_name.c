char *get_file_name(struct dirent *d)
{
  struct stat s;
  char *name;

  if (d == NULL)
    {
     fprintf(stderr, "BUG BUG BUG (got a NULL in get_file_name()).\n");
     return NULL;
    }

  if (stat(d->d_name, &s) < 0)
    {
     perror(d->d_name);
    return NULL;
    }

  if (S_ISDIR(s.st_mode))
    {
     name = (char *)malloc(strlen(d->d_name)+2);
     if (name == NULL)
       return NULL;
     sprintf(name, "%s/", d->d_name);
    }
  else
    {
     name = (char *)strdup(d->d_name);
    }

  return name;
}
