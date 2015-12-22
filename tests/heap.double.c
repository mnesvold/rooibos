double
load ( double * pointer )
{
  return *pointer;
}

void
store ( double * pointer, double value )
{
  *pointer = value;
}

void
stack_allocation ( void )
{
    double pointer;
    store(&pointer, 3.0);
}
