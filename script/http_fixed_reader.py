import socket
from struct import unpack
import asyncio


class HTTPFixedReader:
    MESSAGE_SIZE = 4096
    def __init__(self, host_addr, host_port, post_proc) -> None:
        self.host_addr = host_addr
        self.host_port = host_port
        self.loop = asyncio.get_event_loop()
        self.post_proc = post_proc

    def __aiter__(self):
        """ set a a server that is waiting """
        socket.setdefaulttimeout(.05)
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.bind((self.host_addr, self.host_port))
        self.s.listen()
        return self

    async def __anext__(self):
        try:
            conn, addr = await self.loop.run_in_executor(None, self.s.accept)
            data = b''
            with conn:
                data = (await self.loop.run_in_executor(None, lambda: conn.recv(4500))).split(b'\r\n\r\n')[1]
                await self.loop.run_in_executor(None, lambda: conn.sendall(b'HTTP/1.0 201 Created\r\n'))
                while len(data) < HTTPFixedReader.MESSAGE_SIZE:
                    data += await self.loop.run_in_executor(None, lambda: conn.recv(4500))
                    await self.loop.run_in_executor(None, lambda: conn.sendall(b'HTTP/1.0 201 Created\r\n'))
            return await self.loop.run_in_executor(None, lambda: self.post_proc(data))
        except Exception as e:
            print(type(e), e)


if __name__ == '__main__':
    def decode_time(data):
        return unpack('<QQ', data[-16:])
    
    async def main():
        HOST_ADDR = ""
        reader1 = HTTPFixedReader(HOST_ADDR, 65432, decode_time)
        # reader2 = HTTPFixedReader(HOST_ADDR, 65433, decode_time)
        it1 = reader1.__aiter__()
        # it2 = reader2.__aiter__()
        cnt = 0
        while cnt < 1000:
            cnt += 1
            print(await it1.__anext__())
            # res = await asyncio.gather(it1.__anext__(), it2.__anext__())
            # print(*res)

    asyncio.run(main())
