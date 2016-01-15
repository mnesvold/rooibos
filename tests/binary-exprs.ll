define i32 @add_i32(i32 %a, i32 %b) {
  %1 = add i32 %a, %b
  ret i32 %1
}

define i32 @sub_i32(i32 %a, i32 %b) {
  %1 = sub i32 %a, %b
  ret i32 %1
}

define i32 @sdiv_i32(i32 %a, i32 %b) {
  %1 = sdiv i32 %a, %b
  ret i32 %1
}

define i32 @srem_i32(i32 %a, i32 %b) {
  %1 = srem i32 %a, %b
  ret i32 %1
}

define double @add_doubles(double %a, double %b) {
  %1 = fadd double %a, %b
  ret double %1
}

define double @sub_doubles(double %a, double %b) {
  %1 = fsub double %a, %b
  ret double %1
}

define double @mul_doubles(double %a, double %b) {
  %1 = fmul double %a, %b
  ret double %1
}

define double @div_doubles(double %a, double %b) {
  %1 = fdiv double %a, %b
  ret double %1
}

define double @rem_doubles(double %a, double %b) {
  %1 = frem double %a, %b
  ret double %1
}
