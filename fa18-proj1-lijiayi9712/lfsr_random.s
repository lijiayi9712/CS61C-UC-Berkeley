.data

lfsr:
	.align 4
	.word 0x1

.text

# Implements a 16-bit lfsr
#
# Arguments: None
lfsr_random:
	la t0 lfsr
	lhu a0 0(t0)
	addi t1, x0, 0
	addi t2, x0, 16
	loop: beq t1, t2, end
		srai t3, a0, 0
		srai t4, a0, 2
		xor t5, t3, t4
		srai t3, a0, 3
		xor t4, t5, t3
		srai t3, a0, 5
		xor t5, t3, t4  # highest
        andi t5, t5, 1
		srai a0, a0, 1
		slli t5, t5, 15
		or a0, a0, t5
		addi t1, t1, 1
		j loop
	end: 
	la t0 lfsr
	sh a0 0(t0)
	jr ra