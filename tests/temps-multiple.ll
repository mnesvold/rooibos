define i32 @get_int() {
  ret i32 42
}

define i32 @echo(i32 %n) {
  ret i32 %n
}

define i32 @return_int() {
  %1 = call i32 @get_int()
  %2 = call i32 @echo(i32 %1)
  ret i32 %2
}
