char **get_dir_list(char *dirname, int *num_entries)
{
  int i,size=CHUNK;
  char **table, old_dir[MAXPATHLEN];
  DIR  *dir;
  struct dirent *dirent;

  getcwd(old_dir, MAXPATHLEN);
  if (dirname && chdir(dirname) < 0)
    return NULL;
  
  dir = opendir(".");
  if (dir == NULL)
   {
     chdir(old_dir);
     return NULL;
   }
  
  table = (char **)calloc(size, sizeof(char *));
  if (table == NULL)
   {
     closedir(dir);
     chdir(old_dir);

     return NULL;
   }

  dirent = NULL;   i = 0;
  for(dirent = readdir(dir); dirent != NULL; dirent = readdir(dir))
   {
     table[i] = get_file_name(dirent);

     if (freq_filter_check(table[i])) continue;
         /* continue if table[i] is void or doesn't match the filter */

     i++;
     if (i == size)
      {
	char **table2;

	size *= 2;
	table2 = (char **)realloc(table, size * sizeof(char *));
	if (table2 == NULL)
	 {
	   free_table(table, i);
	   closedir(dir);
	   chdir(old_dir);

	   return NULL;
	 }

	table = table2;
      }
   }

  table[i] = NULL;    /* make sure the table ends with a NULL */

  if (num_entries)
    *num_entries = i;
  
  closedir(dir);
  chdir(old_dir);
  
  return table;
}
