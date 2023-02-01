.data
LED_ADDR:
    .word 0xFFFFFC60
.text
    lw $1, LED_ADDR($0)
    sw $1, 0($0)
