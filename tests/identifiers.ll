define void @identifiers() {
  call void @lettersAndDigitsAreNotEnc0ded()
  call void @neither_are_underscores()
  call void @but_$_is()
  call void @as-are-hyphens()
  call void @"also spaces"()
  call void @and.periods()
  call void @"and_things_like_\01"()
  ret void
}

declare void @lettersAndDigitsAreNotEnc0ded()
declare void @neither_are_underscores()
declare void @but_$_is()
declare void @as-are-hyphens()
declare void @"also spaces"()
declare void @and.periods()
declare void @"and_things_like_\01"()
