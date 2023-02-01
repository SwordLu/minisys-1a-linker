.data
  f0_num1:  	
.word 0x00000055
  f0_num2:  	
.word 0x000000aa
  f0_num3:  	
.word 0x00000000
  f1_num1:  	
.word 0x00000055
.text
f0_start:
   lui   $1,0xffff
   ori $28,$1,0xf000
f0_k1:
   lw $2,f0_num1($0)    
   lw $3,f0_num2($0)      
   add $1,$2,$3        
   sw $1,f0_num3($0)      
   subu $4,$3,$2       
   slt $4,$2,$1        
   and $1,$3,$7        
   or $6,$2,$1         
   xor $7,$2,$3        
   nor $6,$5,$1        
   beq $3,$2,-4        
f0_gh:	
   sub $2,$2,$5      
   sw   $2, 0xc50($28) 
   bne $5,$2,f0_gh        
   nop                 
   beq $1,$1,f0_ty        
   nop                 
f0_ty:
   jal f0_jj              
   nop                 
   j f0_mm
   nop
f0_jj:
   jr $31
   nop
f0_mm:
   addi $2,$0,0x99
   ori $3,$0,0x77
   sll $3,$2,4
   srl $3,$2,4
   srlv $3,$2,$1
   lui $6,0x9988
   sra $7,$6,4
   addi $2,$0,0
   sw   $2, 0xc50($28)
   addi $3,$0,2
   sub  $1,$2,$3
   j f0_k1
f1___start:
 	addiu 	$sp, $sp, 0 
 	sw 	$ra, 0($sp) 
 	sw 	$s8, -4($sp) 
 	ori 	$s8, $sp, 0 
 	jal 	f1_main 
 	nop 
 	lw 	$ra, 0($sp) 
 	addiu 	$sp, $sp, 0 
 	nop 
f1_main: 
 	addiu 	$sp, $s8, -16 
 	sw 	$zero, -8($s8) 
f1_entry.0: 
 	nop
f1_while.cond.0: 
 	ori 	$t1, $zero, 1 
 	bne 	$t1, $zero, f1_while.body.0 
 	nop 
 	j 	f1_while.end.0 
 	nop 
f1_while.body.0: 
 	addiu 	$t1, $zero, 0xfd10 
 	lw 	$t3, -12($s8) 
 	ori 	$t2, $zero, 10 
 	srlv 	$t2, $t3, $t2 
 	sw 	$t2, 0($t1) 
 	lw 	$t2, -12($s8) 
 	ori 	$t1, $zero, 1 
 	addu 	$t1, $t2, $t1 
 	sw 	$t1, -12($s8) 
 	j 	f1_while.cond.0 
 	nop 
f1_while.end.0: 
 	ori 	$t1, $zero, 0 
 	sw 	$t1, -8($s8) 
 	j 	f1_ret.0 
 	nop 
f1_ret.0: 
 	lw 	$t1, -8($s8) 
 	ori 	$v0, $t1, 0 
 	ori 	$sp, $s8, 0 
 	lw 	$ra, 0($s8) 
 	lw 	$s8, -4($s8) 
 	jr 	$ra 
 	nop 
 	nop 
 	j 148
