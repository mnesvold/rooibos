define i32 @return_coercion() {
  ret i32 17
}

define void @param_coercion(i32 %n) {
  ret void
}

define void @arg_coercion() {
  call void @param_coercion(i32 17)
  ret void
}

define void @locals() {
  %1 = call i32 @return_coercion()
  call void @param_coercion(i32 %1)
  ret void
}
