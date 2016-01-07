void loop_forever()
{
top:
  goto top;
}

void one_thing(void) {}
void another_thing(void) {}

void choice(int x)
{
  if(x)
  {
    one_thing();
  }
    else
  {
    another_thing();
  }
}
