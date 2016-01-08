define i32 @get_lesser(i32 %m, i32 %n) {
  %1 = call i32 @__rooibos_stdlib_Math__min(i32 %m, i32 %n)
  ret i32 %1
}

define void @call_missing() {
  call void @__rooibos_stdlib_not__a__real__call()
  ret void
}

declare i32 @__rooibos_stdlib_Math__min(i32, i32)
declare void @__rooibos_stdlib_not__a__real__call()
