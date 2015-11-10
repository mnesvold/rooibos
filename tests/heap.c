int
load ( int * pointer )
{
  return *pointer;
}

void
store ( int * pointer, int value )
{
  *pointer = value;
}

void
stack_allocation ( void )
{
    int pointer;
    store(&pointer, 3);
}
