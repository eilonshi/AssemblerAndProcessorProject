main:
    add $sp, $zero, $zero, 512	            # set $sp = 512
    lw $a0, $imm, $zero, 0x100	            # get n from address 0x100
    lw $a1, $imm, $zero, 0x101	            # get k from address 0x101
    jal $ra, $imm, $zero, binom	            # $v0 = binom(n,k)
    sw $v0, $imm, $zero, 0x102	            # store binom(n,k) in 0x102
    halt $zero, $zero, $zero, 0	            # halt

binom:
	sub $sp, $sp, $imm, 6  	    	        # adjust stack for 6 items
	sw $ra, $sp, $imm, 5                    # store $ra return address
	sw $v0, $sp, $imm, 4                    # store $v0
	sw $s0, $sp, $imm, 3                    # store $s0
	sw $s1, $sp, $imm, 2                    # store $s1
	sw $a0, $sp, $imm, 1                    # store n
	sw $a1, $sp, $imm, 0                    # store k

	add $t0, $zero, $zero		            # $t0 = 0
	beq $zero, $a1, $imm, L1		        # if (k == 0) jump to L1
	beq $a0, $a1, $imm, L1		            # if (n == k) jump to L1
	sub $a0, $a0, $imm, 1		            # calc n - 1
	sub $a1, $a1, $imm, 1		            # calc k - 1
	jal $ra, $imm, $zero, binom             # $v0 = bin(n - 1, k - 1)

	add $s0, $v0, $zero                     # $s0 = bin(n - 1, k - 1)
	lw $a0, $sp, $imm, -1                   # restore $a0 = n
	sub $a0, $a0, $imm, 1                   # calculate n - 1
	lw $a1, $sp, $zero                      # restore $a1 = k
	jal $ra, $imm, $zero, binom  	        # $v0 = bin(n - 1, k)

	add $s1, $v0, $zero                     # $s1 = bin(n - 1, k)
	add $v0, $s1, $s0, $zero                # $v0 = bin(n - 1, k - 1) + bin(n - 1, k)
	
    lw $ra, $sp, $zero                      # save $ra return address
	lw $v0, $sp, $imm, 1                    # restore $v0
	lw $s0, $sp, $imm, 2                    # restore $s0
	lw $s1, $sp, $imm, 3                    # restore $s1
	lw $a0, $sp, $imm, 4                    # restore argument n
	lw $a1, $sp, $imm, 5                    # restore argument k
    add $sp, $sp, $imm, 6                   # pop 6 items from stack
	beq $zero, $zero, $ra                   # jump to $ra - end function

L1:
	add $v0, $zero, $imm, 1                 # bin(n, k) = 1
	add $sp, $sp, $imm, 7                   # pop 7 items from stack
    beq $zero, $zero, $ra                   # jump to $ra - end function

.word 0x100 4
.word 0x101 0
