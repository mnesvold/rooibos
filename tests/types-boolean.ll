define i1 @return_coercion() {
  ret i1 1
}

define void @param_coercion(i1 %n) {
  ret void
}

define void @arg_coercion() {
  call void @param_coercion(i1 1)
  ret void
}

define void @locals() {
  %1 = call i1 @return_coercion()
  call void @param_coercion(i1 %1)
  ret void
}
