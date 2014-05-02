lgit.so: lgit.c src/common.c src/branch.c src/tree.c src/commit.c src/tree_builder.c
	gcc --shared -fPIC -o lgit.so \
		lgit.c src/common.c src/branch.c src/tree.c src/commit.c src/tree_builder.c \
		-L /usr/local/lib -I /usr/include \
		-g -Wall -Wextra -Werror -std=c11 \
		-lc -lgit2 -llua 

