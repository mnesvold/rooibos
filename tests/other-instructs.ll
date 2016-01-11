define void @compare_ints(i32 %x, i32 %y) {
  %1 = icmp eq i32 %x, %y
  %2 = icmp ne i32 %x, %y
  %3 = icmp sgt i32 %x, %y
  %4 = icmp sge i32 %x, %y
  %5 = icmp slt i32 %x, %y
  %6 = icmp sle i32 %x, %y
  ret void
}

define i32 @select_int() {
  %1 = select i1 1, i32 10, i32 20
  ret i32 %1
}

define double @select_double() {
  %1 = select i1 0, double 10.0, double 20.0
  ret double %1
}
