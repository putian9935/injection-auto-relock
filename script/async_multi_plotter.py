import tkinter as tk
import asyncio
import matplotlib.pyplot as plt
from http_fixed_reader import HTTPFixedReader
from struct import unpack
import matplotlib as mpl
mpl.rcParams['toolbar'] = 'None'

def extract_number(data: bytearray):
    return unpack('<512d', data)


class AsyncMultiPlotter:
    # web address constants
    HOST_ADDR = ""
    CH1_PORT = 65432
    CH2_PORT = 65433

    def __init__(self) -> None:
        self.fig, self.ax = plt.subplots()
        self.fig.canvas.manager.set_window_title('Scope')
        n_channel = 2
        self.channels = [0] * n_channel
        self.lines = []
        for _ in range(n_channel):
            self.lines.append(plt.plot([], [])[0])
        self.x = list(range(512))

        # fix limit
        self.ax.set_ylim(-.1, 1.1)
        self.ax.set_xlim(-1, 513)

        self.loop = asyncio.get_event_loop()

        reader1 = HTTPFixedReader(
            AsyncMultiPlotter.HOST_ADDR,
            AsyncMultiPlotter.CH1_PORT,
            extract_number
        )
        reader2 = HTTPFixedReader(
            AsyncMultiPlotter.HOST_ADDR,
            AsyncMultiPlotter.CH2_PORT,
            extract_number
        )
        it1 = reader1.__aiter__()
        it2 = reader2.__aiter__()
        self.feeders = [it1.__anext__, it2.__anext__]
        print('Initialize done!')

    async def animate(self):
        try:
            while True:
                if not plt.fignum_exists(self.fig.number):
                    break
                # in case it is modified
                channels = self.channels[:]  
                tasks = [
                    asyncio.create_task(it())
                    for on, it in zip(channels, self.feeders)
                    if on
                ]
                cnt = 0
                for on, line in zip(channels, self.lines):
                    if on:
                        res = await tasks[cnt]
                        if tasks[cnt].exception() is not None:
                            print(tasks[cnt].get_stack)
                            cnt += 1
                        line.set_data(self.x, res)
                        cnt += 1
                    else:
                        line.set_data([], [])

                plt.pause(0.001)
                await asyncio.sleep(0)
        except Exception as e:
            # print(res)
            # print(len(res))
            print('***')
            print(sum(channels))
            print(len(tasks))
            print(channels)
            print(e)


class AsyncMultiPlotterController(tk.Tk):
    def __init__(self) -> None:
        super().__init__()
        self.protocol("WM_DELETE_WINDOW", self.close)
        self.plotter = AsyncMultiPlotter()
        self.task = [
            asyncio.create_task(self.aupdate()),
            asyncio.create_task(self.plotter.animate())
        ]

    async def aupdate(self):
        while True:
            self.update()
            try:
                await asyncio.sleep(0.01)
            except:
                return

    def close(self):
        for t in self.task:
            if not t.cancelled() and not t.done():
                t.cancel()
        self.destroy()

    def toggle(self, ch):
        self.plotter.channels[ch] ^= 1


async def main():
    window = AsyncMultiPlotterController()
    button = tk.Button(text="CH1", width=25, height=5,
                       command=lambda: window.toggle(0))
    button.grid(row=0, column=0, sticky='nsew')

    button2 = tk.Button(text="CH2", width=25, height=5,
                        command=lambda: window.toggle(1))
    button2.grid(row=0, column=1, sticky='nsew')
    await window.task[0]

if __name__ == '__main__':
    asyncio.run(main())
