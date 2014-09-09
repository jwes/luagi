FILES := luagi.o \
		repository.o \
		common.o \
		branch.o \
		tree.o \
		commit.o \
		tree_builder.o \
		clone.o \
		remote.o \
      transport.o \
		status.o \
		diff.o \
		index.o \
		object.o \
		checkout.o \
		stash.o \
		merge.o \
		reference.o \
		oid.o \
		types.o \
		blob.o \
		cherrypick.o \
		push.o \
		reset.o \
		tag.o \
		submodule.o \
		graph.o \
		note.o \
		blame.o \
		ignore.o \
#revert.o \
#

CFLAGS:=-fPIC -g -Wall -Wextra -Werror -std=c11
LIBS:=-lc -lgit2 -llua 
VPATH:=src
CC=gcc

all: luagi.so test
test:
	lua tst/luagi.lua

luagi.so: $(FILES)
	$(CC) --shared -o $@ $^ $(LIBS)
	

clean:
	rm $(FILES)
	rm luagi.so
