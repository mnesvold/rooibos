define double @return_coercion() {
  ret double 1.700000e+01
}

define void @param_coercion(double %n) {
  ret void
}

define void @arg_coercion() {
  call void @param_coercion(double 1.700000e+01)
  ret void
}

define void @locals() {
  %1 = call double @return_coercion()
  call void @param_coercion(double %1)
  ret void
}
