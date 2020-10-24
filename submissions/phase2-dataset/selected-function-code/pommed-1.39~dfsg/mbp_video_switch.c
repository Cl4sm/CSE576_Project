void
mbp_video_switch(void)
{
  struct passwd *pw;
  char *vsw = NULL;

  int ret;

  if (vsw_user == NULL)
    {
      pw = getpwuid(getuid());
      if (pw == NULL)
        {
          fprintf(stderr, "Could not get user information\n");

          return;
        }

      vsw_user = (char *) malloc(strlen(pw->pw_dir) + strlen(VIDEO_SWITCH_USER) + 1);
      if (vsw_user == NULL)
        {
          fprintf(stderr, "Could not allocate memory\n");

          return;
        }

      strncpy(vsw_user, pw->pw_dir, strlen(pw->pw_dir) + 1);
      strncat(vsw_user, VIDEO_SWITCH_USER, strlen(VIDEO_SWITCH_USER));
    }

  if (access(vsw_user, R_OK | X_OK) == 0)
    {
      vsw = vsw_user;
    }
  else if (access(VIDEO_SWITCH_SYSTEM, R_OK | X_OK) == 0)
    {
      vsw = VIDEO_SWITCH_SYSTEM;
    }
  else
    {
      fprintf(stderr, "No video switch script available\n");
      return;
    }

  ret = fork();
  if (ret == 0) /* exec video switch script */
    {
      execl(vsw, "videoswitch", NULL);

      fprintf(stderr, "Could not execute video switch script: %s", strerror(errno));
      exit(1);
    }
  else if (ret == -1)
    {
      fprintf(stderr, "Could not fork: %s\n", strerror(errno));
      return;
    }
}