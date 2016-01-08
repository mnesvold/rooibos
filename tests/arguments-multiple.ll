define i32 @echo(i32 %m, i32 %n) {
  ret i32 %n
}

define i32 @pass_to_echo(i32 %n) {
  %1 = call i32 @echo(i32 %n, i32 %n)
  ret i32 %1
}
