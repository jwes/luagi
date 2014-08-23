FILES := src/luagi.c \
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
		src/index.c \
		src/object.c \
		src/checkout.c \
		src/stash.c

luagi.so: $(FILES)
	gcc --shared -fPIC -o luagi.so \
	$(FILES) \
		-g -Wall -Wextra -Werror -std=c11 \
		-lc -lgit2 -llua 

