define i32 @select_int() {
  %1 = select i1 1, i32 10, i32 20
  ret i32 %1
}

define double @select_double() {
  %1 = select i1 0, double 10.0, double 20.0
  ret double %1
}
