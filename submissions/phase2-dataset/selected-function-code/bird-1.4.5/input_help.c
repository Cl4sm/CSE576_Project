input_help(int arg, int key UNUSED)
{
  int i, in_string, in_bracket;

  if (arg != 1)
    return rl_insert(arg, '?');

  in_string = in_bracket = 0;
  for (i = 0; i < rl_point; i++)
    {
   
      if (rl_line_buffer[i] == '"')
	in_string = ! in_string;
      else if (! in_string)
        {
	  if (rl_line_buffer[i] == '[')
	    in_bracket++;
	  else if (rl_line_buffer[i] == ']')
	    in_bracket--;
        }
    }

  /* `?' inside string or path -> insert */
  if (in_string || in_bracket)
    return rl_insert(1, '?');

  rl_begin_undo_group();		/* HACK: We want to display `?' at point position */
  rl_insert_text("?");
  rl_redisplay();
  rl_end_undo_group();
  input_start_list();
  cmd_help(rl_line_buffer, rl_point);
  rl_undo_command(1, 0);
  input_stop_list();
  return 0;
}
