  long
b_goto_line(buffer, number)
  Buffer *buffer;
  long number;
{
  BufferBlock *i;
  long newlines = 0, blocks = 0;
  char *c;

  if (number == 1) return 0;
  if (buffer == last_buffer && number == last_number) return last_position;
  for (i = buffer -> first_block; i; i = i -> next_block) {
    for (c = i -> data; c < i -> data + buffer -> blocksize; c++)
      if (*c == '\n')
        if (++newlines == number - 1) {
          long block_position = blocks * buffer -> blocksize;
          last_position = block_position + (long)c - (long)(i -> data) + 1;
          last_number = number;
          last_buffer = buffer;
          last_length = -1;
          return last_position;
        }
    blocks++;
  }
  return -1;
}
