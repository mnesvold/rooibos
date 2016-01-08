define void @ping() {
  ret void
}

define void @reping() {
  call void @ping()
  call void @ping()
  ret void
}
