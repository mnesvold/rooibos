define void @unconditional_branch() {
  br label %1

; <label>:1                                       ; preds = %0
  ret void
}

define i32 @conditional_branch(i32 %x) {
  %1 = icmp ne i32 %x, 0
  br i1 %1, label %2, label %3

; <label>:2                                       ; preds = %0
  ret i32 0

; <label>:3                                       ; preds = %0
  ret i32 %x
}
