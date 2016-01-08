; Arguments and parameters: correct type annotations on formal parameters,
; passed arguments, and return types.

define i32 @echo(i32 %n) {
  ret i32 %n
}

define i32 @multiecho(i32 %m, i32 %n) {
  ret i32 %m
}

define i32 @pass_to_echo(i32 %n) {
  %1 = call i32 @echo(i32 %n)
  ret i32 %1
}

define i32 @pass_to_multiecho(i32 %n) {
  %1 = call i32 @multiecho(i32 %n, i32 %n)
  ret i32 %1
}
