%struct.foo = type { i32, i32, i32, i32 }

define i32* @singleGEP(i32* %array, i32 %index) {
  %1 = getelementptr inbounds i32, i32* %array, i32 %index
  ret i32* %1
}

define i32* @doubleGEP(%struct.foo* %foo) {
  %1 = getelementptr inbounds %struct.foo, %struct.foo* %foo, i32 0, i32 3
  ret i32* %1
}

define %struct.foo* @unusualGEPSize(%struct.foo* %foo) {
  %1 = getelementptr inbounds %struct.foo, %struct.foo* %foo, i32 1
  ret %struct.foo* %1
}
