local skynet = require "skynet"
local socket = require "skynet.socket"
local string = require "string"
local websocket = require "websocket"
local httpd = require "http.httpd"
local urllib = require "http.url"
local sockethelper = require "http.sockethelper"
-- local mylib = require "mytestlib"

local num = 0


function lua_test()
    skynet.error("--- lua_test function call")
end

local handler = {}
function handler.on_open(ws)
    skynet.error(string.format("%d::open", ws.id))
end

function handler.on_message(ws, message)
    skynet.error(string.format("%d receive:%s", ws.id, message))
    skynet.error("!!!!! comming !!!!! " .. message)
    ws:send_text(message .. "from server")
    ws:close()
end

function handler.on_close(ws, code, reason)
    print(string.format("%d close:%s  %s", ws.id, code, reason))
end

local function handle_socket(id)
    -- limit request body size to 8192 (you can pass nil to unlimit)
    skynet.error("ready to handler socket : " .. id)
    local code, url, method, header, body = httpd.read_request(sockethelper.readfunc(id), 8192)
    skynet.error("code :" .. code)
    skynet.error("url : " .. url)
    skynet.error("method : " .. method)
    if code then
        if url == "/ws" then
            --num = num + 1
            --skynet.error("mylib.add result = " .. mylib.add(1, 2))
            --local tmp = {"123", false, 123};
            --mylib.stack_dump("123", false, 123)
            --mylib.call_lua_func()
            --mylib.callback_test2(10, handler.on_test, 1985)
            skynet.error("begin websocket handler")
            local ws = websocket.new(id, header, handler)
            ws:start()
        end
    end
end

skynet.start(function()
    local address = "0.0.0.0:8001"
    skynet.error("Listening "..address)
    local id = assert(socket.listen(address))
    skynet.error("!!!!server listen id :" .. id)
    socket.setssl(id)
    socket.start(id , function(cli_id, addr)
        skynet.error("!!!!id-------" .. cli_id)
        socket.start(cli_id)
        pcall(handle_socket, cli_id)
    end)
end)
