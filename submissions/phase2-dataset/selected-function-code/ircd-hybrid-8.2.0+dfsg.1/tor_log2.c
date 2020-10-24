static int
tor_log2(uint64_t u64)
{
  int r = 0;

  if (u64 >= (1LLU << 32))
  {
    u64 >>= 32;
    r = 32;
  }

  if (u64 >= (1LLU << 16))
  {
    u64 >>= 16;
    r += 16;
  }

  if (u64 >= (1LLU <<  8))
  {
    u64 >>= 8;
    r += 8;
  }

  if (u64 >= (1LLU <<  4))
  {
    u64 >>= 4;
    r += 4;
  }

  if (u64 >= (1LLU <<  2))
  {
    u64 >>= 2;
    r += 2;
  }

  if (u64 >= (1LLU <<  1))
  {
    u64 >>= 1;
    r += 1;
  }

  return r;
}
