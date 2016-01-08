define double @load(double* %pointer) {
  %1 = load double, double* %pointer, align 4
  ret double %1
}

define void @store(double* %pointer, double %value) {
  store double %value, double* %pointer, align 4
  ret void
}

define void @stack_allocation() {
  %pointer = alloca double, align 8
  call void @store(double* %pointer, double 3.000000e+00)
  ret void
}
