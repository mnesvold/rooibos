void ping() __attribute__((noinline));

void
ping ( void )
{
}

void
reping ( void )
{
  ping();
  ping();
}
