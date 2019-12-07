.text

# Decodes a quadtree to the original matrix
#
# Arguments:
#     quadtree (qNode*)
#     matrix (void*)
#     matrix_width (int)
#
# Recall that quadtree representation uses the following format:
#     struct qNode {
#         int leaf;
#         int size;
#         int x;
#         int y;
#         int gray_value;
#         qNode *child_NW (20), *child_NE, *child_SE, *child_SW;
#     }

quad2matrix:
    beq a0, x0, end #the qNode
    lw t0, 0(a0)  #leaf
    li t1, 1
    beq t0, t1, leaf_fill #check leaf, if yes, go fill the matrix
    addi sp, sp, -8
    sw ra, 0(sp)
    sw a0, 4(sp)
    lw a0, 20(a0) #child_NW
    jal ra quad2matrix
    lw a0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 8
    addi sp, sp, -8
    sw ra, 0(sp)
    sw a0, 4(sp)
    lw a0, 24(a0) #child_NE
    jal ra quad2matrix
    lw a0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 8
    addi sp, sp, -8
    sw ra, 0(sp)
    sw a0, 4(sp)
    lw a0, 28(a0) #child_SE
    jal ra quad2matrix
    lw a0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 8
    addi sp, sp, -8
    sw ra, 0(sp)
    sw a0, 4(sp)
    lw a0, 32(a0) #child_SW
    jal ra quad2matrix
    lw a0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 8
    jr ra
leaf_fill:
    addi sp, sp, -4
    sw ra, 0(sp)
    lw t1, 8(a0) #x
    lw t2, 12(a0) #y
    lw t3, 4(a0) #size
    add t4, t1, t3 #x upper bound
    add t5, t2, t3 #y upper bound
    loopy: beq t2, t5, end   #y
        loopx: beq t1, t4, innerend #x
             mul t6, t2, a2
             add t6, t6, t1
             add t6, a1, t6
             lw t0, 16(a0)  #grey color
             sb t0, 0(t6)
             addi t1, t1, 1
             #bne t2, t5 loopx
             jal loopx
    innerend: lw t1, 8(a0) #reset x
              addi t2, t2, 1 #increment y
              jal loopy
end:lw ra, 0(sp) 
    addi sp, sp, 4
    jr ra