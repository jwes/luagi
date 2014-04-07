lgit.so: lgit.c src/common.c
	gcc --shared -fPIC -o lgit.so \
		lgit.c src/common.c \
		-L /usr/local/lib -I /usr/include \
		-g -Wall -Wextra -Werror -std=c11 \
		-lc -lgit2 -llua 

