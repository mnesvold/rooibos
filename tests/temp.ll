define i32 @get_int() {
  ret i32 42
}

define i32 @return_int() {
  %1 = call i32 @get_int()
  ret i32 %1
}

define i32 @pick(i32 %m, i32 %n) {
  ret i32 %m
}

define i32 @multiple_temps() {
  %1 = call i32 @get_int()
  %2 = call i32 @return_int()
  %3 = call i32 @pick(i32 %1, i32 %2)
  ret i32 %3
}
