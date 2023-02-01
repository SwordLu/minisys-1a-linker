.data
LED_ADDR:
    .word 0xFFFFFC60
.text
repeat_flag:
    add $1, $2, $3
    j 0
