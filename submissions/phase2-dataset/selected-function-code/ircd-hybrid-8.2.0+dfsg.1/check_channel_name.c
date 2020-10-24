int
check_channel_name(const char *name, const int local)
{
  const char *p = name;

  assert(name != NULL);

  if (!IsChanPrefix(*p))
    return 0;

  if (!local || !ConfigChannel.disable_fake_channels)
  {
    while (*++p)
      if (!IsChanChar(*p))
        return 0;
  }
  else
  {
    while (*++p)
      if (!IsVisibleChanChar(*p))
        return 0;
  }

  return p - name <= CHANNELLEN;
}
