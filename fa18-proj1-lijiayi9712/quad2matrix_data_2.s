.data
# Quadtree Node representation
# Recall that quadtree representation uses the following format:
#	struct qNode {
#		int leaf;
#		int size;
#		int x;
#		int y;
#		int gray_value;
#		qNode *child_NW, *child_NE, *child_SE, *child_SW;
#	}

q00:   .word  0, 4, 0, 0,  -1, 0, 0, 0, 0

qx00:  .word  1, 2, 0, 0,   1, 0, 0, 0, 0
qx20:  .word  1, 2, 2, 0,   1, 0, 0, 0, 0
qx22:  .word  1, 2, 2, 2,   1, 0, 0, 0, 0
qx02:  .word  1, 2, 0, 2,   2, 0, 0, 0, 0

