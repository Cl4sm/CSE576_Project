static int
input_complete(int arg UNUSED, int key UNUSED)
{
  static int complete_flag;
  char buf[256];

  if (rl_last_func != input_complete)
    complete_flag = 0;
  switch (cmd_complete(rl_line_buffer, rl_point, buf, complete_flag))
    {
    case 0:
      complete_flag = 1;
      break;
    case 1:
      rl_insert_text(buf);
      break;
    default:
      complete_flag = 1;
#ifdef HAVE_RL_DING
      rl_ding();
#endif
    }
  return 0;
}
