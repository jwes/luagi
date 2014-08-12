FILES := src/wien.c \
		src/common.c \
		src/branch.c \
		src/tree.c \
		src/commit.c \
		src/tree_builder.c \
		src/clone.c \
		src/remote.c \
      src/transport.c \
		src/status.c \
		src/diff.c \
		src/index.c

wien.so: $(FILES)
	gcc --shared -fPIC -o wien.so \
	$(FILES) \
		-g -Wall -Wextra -Werror -std=c11 \
		-lc -lgit2 -llua 

