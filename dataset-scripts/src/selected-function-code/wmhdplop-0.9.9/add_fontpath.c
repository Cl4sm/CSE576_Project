static void add_fontpath(const char *path, int recurse) {
  struct stat st;
    
  if (stat(path,&st) != 0 ||
      !S_ISDIR(st.st_mode)) return;
  if (recurse > 3) return; /* prevent scanning of whole hd/infinite recursions in case of a bad symlink */
  printf("add font path: '%s'\n", path);
  imlib_add_path_to_font_path(path);
  if (recurse) {
    DIR *d = opendir(path);
    struct dirent *de;
    if (!d) return;
    while ((de = readdir(d))) {
      char s[1024];
      if (strcmp(de->d_name,".") == 0 ||
          strcmp(de->d_name,"..") == 0) continue;
      snprintf(s,sizeof s,"%s/%s",path, de->d_name);
      add_fontpath(s,recurse+1);
    }
    closedir(d);
  }
}