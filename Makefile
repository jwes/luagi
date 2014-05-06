FILES := lgit.c \
		src/common.c \
		src/branch.c \
		src/tree.c \
		src/commit.c \
		src/tree_builder.c \
		src/clone.c

lgit.so: $(FILES)
	gcc --shared -fPIC -o lgit.so \
	$(FILES) \
		-g -Wall -Wextra -Werror -std=c11 \
		-lc -lgit2 -llua 

