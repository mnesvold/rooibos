define i32 @get_lesser(i32 %m, i32 %n) {
  %1 = call i32 @__rooibos_stdlib_Math__min(i32 %m, i32 %n)
  ret i32 %1
}

declare i32 @__rooibos_stdlib_Math__min(i32, i32)
