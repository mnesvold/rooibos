int
get_int ( void )
{
  return 42;
}

int
echo ( int n )
{
  return n;
}

int
return_int ( void )
{
  return echo(get_int());
}
