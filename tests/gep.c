int * singleGEP(int * array, int index) {
  return array + index;
}

struct foo {
  int w;
  int x;
  int y;
  int z;
};

int * doubleGEP(struct foo * foo) {
  return &(foo->z);
}

struct foo * unusualGEPSize(struct foo * foo) {
  return foo + 1;
}
