define void @loop_forever() {
  br label %1

; <label>:1                                       ; preds = %0, %1
  br label %1
                                                  ; No predecessors!
  ret void
}

define void @one_thing() {
  ret void
}

define void @another_thing() {
  ret void
}

define void @choice(i32 %x) {
  %1 = icmp ne i32 %x, 0
  br i1 %1, label %2, label %3

; <label>:2                                       ; preds = %0
  call void @one_thing()
  br label %4

; <label>:3                                       ; preds = %0
  call void @another_thing()
  br label %4

; <label>:4                                       ; preds = %2, %3
  ret void
}
