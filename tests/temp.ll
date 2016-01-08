define i32 @get_int() {
  ret i32 42
}

define i32 @return_int() {
  %1 = call i32 @get_int()
  ret i32 %1
}
