CC ?= g++
CFLAGS = -g -O2 -std=c++11 -Wall -I$(LUA_INC)
SHARED := -fPIC --shared

TARGET = myLualib.so
LUA_CLIB_PATH = clib

# 引入lua头文件

LUA_INC ?= ../../3rd/lua

start: $(TARGET)

$(TARGET) : myLualib.c | $(LUA_CLIB_PATH)
	$(CC) $(CFLAGS) $(SHARED) $^ -o $@

clean:
	rm -fr $(TARGET)
	rm -rf $(LUA_CLIB_PATH)

$(LUA_CLIB_PATH) :
	mkdir $(LUA_CLIB_PATH)