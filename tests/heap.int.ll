define i32 @load(i32* %pointer) {
  %1 = load i32, i32* %pointer, align 4
  ret i32 %1
}

define void @store(i32* %pointer, i32 %value) {
  store i32 %value, i32* %pointer, align 4
  ret void
}

define void @stack_allocation() {
  %pointer = alloca i32, align 4
  call void @store(i32* %pointer, i32 3)
  ret void
}
