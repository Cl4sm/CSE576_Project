round_to_power_of_2(uint64_t u64)
{
  int lg2;
  uint64_t low;
  uint64_t high;

  if (u64 == 0)
    return 1;

  lg2 = tor_log2(u64);
  low = 1LLU << lg2;

  if (lg2 == 63)
    return low;

  high = 1LLU << (lg2 + 1);
  if (high - u64 < u64 - low)
    return high;
  else
    return low;
}
