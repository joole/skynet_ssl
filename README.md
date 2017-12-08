## Skynet_SSL

根据云风的skynet修改添加了SSL的支持，其中还有一些问题

+ 证书文件已经写死路径，尚未开启配置化，文件名字为snakeoil.crt
+ 接收的缓冲还有一些问题正在修复
+ 发送的缓冲过大引发内存不优化的问题
+ 编译需要OPenSSL的支持，请提前准备

## Build

For Linux, install autoconf first for jemalloc:

```
git clone https://github.com/cloudwu/skynet.git
cd skynet
make 'PLATFORM'  # PLATFORM can be linux, macosx, freebsd now
```

Or you can:

```
export PLAT=linux
make
```

For FreeBSD , use gmake instead of make.

## Test

Run these in different consoles:

```
./skynet examples/config	# Launch first skynet node  (Gate server) and a skynet-master (see config for standalone option)
./3rd/lua/lua examples/client.lua 	# Launch a client, and try to input hello.
```

## About Lua version

Skynet now uses a modified version of lua 5.3.4 ( https://github.com/ejoy/lua/tree/skynet ) for multiple lua states.

You can also use official Lua versions, just edit the Makefile by yourself.

## How To Use (Sorry, Only in Chinese now)

* Read Wiki for documents https://github.com/cloudwu/skynet/wiki
* The FAQ in wiki https://github.com/cloudwu/skynet/wiki/FAQ
