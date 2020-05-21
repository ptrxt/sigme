
SRCDIRS =\
. \
../receivers \
../sources \
../../src

SRC= \
main.c \
install_signals.c \
humid.c \
stdout_broadcaster.c \
sgm.c \
sgm_signal.c 

INCLUDES = \
-I. \
-I../signals \
-I../receivers \
-I../sources \
-I../../inc \
-I/usr/include/gio-unix-2.0 \
-I/usr/include/glib-2.0 \
-I/usr/lib/x86_64-linux-gnu/glib-2.0/include

OBJS := $(SRC:%.c=./build/%.o)
LIBS := -lpthread -lgthread-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
LIBDIR = -L.

VPATH = $(SRCDIRS)

all: builddir application

builddir:
	mkdir -p build

application: $(OBJS)
	g++ $(LIBDIR) -o ./build/stdout_broadcaster $(OBJS) $(LIBS) 

./build/%.o: %.c
	gcc -c $(INCLUDES) $^ -o $@



clean:
	cd build && rm -rf *
