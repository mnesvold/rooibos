int
return_coercion ( void )
{
  return 17;
}

void
param_coercion ( int n )
{
}

void
arg_coercion ( void )
{
  param_coercion(17);
}

void
locals ( void )
{
  param_coercion(return_coercion());
}
