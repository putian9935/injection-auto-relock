# Import tkinter library
from tkinter import *
import tkinter.font as font
import asyncio
from collections import defaultdict
from async_multi_plotter import AsyncMultiPlotter


async def killall(channel):
    try:
        p = await asyncio.create_subprocess_shell(
            'ssh slave%d-lock "killall sweep lock sweep-upload"' % channel,
            stderr=asyncio.subprocess.DEVNULL, stdin=asyncio.subprocess.PIPE
        )
        # no idea why, but it works (bad return value? )
        await p.communicate(b'\n\n')
        print("slave%d-lock: unlocked" % channel)
    except Exception as e:
        print(e)


async def monitor(proc, channel):
    while True:
        await asyncio.sleep(0.2)
        try:
            cout = await proc.stdout.readline()
            if len(cout.strip()) > 0:
                print("slave%d-lock: " % channel, cout.strip().decode())
        except:
            proc.kill()
            break


async def sweep(channel, chs):
    try:
        proc = await asyncio.create_subprocess_exec('ssh',  "slave%d-lock" % channel, '"/root/injection-auto-relock/build/sweep-upload"', stdout=asyncio.subprocess.PIPE, stderr=asyncio.subprocess.PIPE)
        print('slave%d-lock: sweep at' % channel, proc, flush=True)
        chs[channel-2] = 1
        await monitor(proc, channel)
    except Exception as e:
        print('sweep', e)


async def lock(channel):
    proc = await asyncio.create_subprocess_exec('ssh',  "slave%d-lock" % channel, '"/root/injection-auto-relock/build/lock"', stdout=asyncio.subprocess.PIPE, stderr=asyncio.subprocess.PIPE)
    print('slave%d-lock: lock at' % channel, proc)
    await monitor(proc, channel)


class MyTk(Tk):
    def __init__(self):
        super().__init__()
        self.protocol("WM_DELETE_WINDOW", self.close)
        self.plotter = AsyncMultiPlotter()
        self.task = [
            asyncio.create_task(self.aupdate()),
            asyncio.create_task(self.plotter.animate())
        ]
        self.channel_task = defaultdict(list)

    async def aupdate(self):
        """ the Tk loop """
        while True:
            self.update()
            try:
                # refresh rate at 0.01
                await asyncio.sleep(0.01)
            except:
                return

    def close(self):
        for t in self.task:
            if not t.cancelled() and not t.done():
                t.cancel()
        for l in self.channel_task.values():
            for t in l:
                if not t.cancelled() and not t.done():
                    t.cancel()
        self.destroy()

    async def action(self, x, y):
        """ action for each channel

        Parameters
        ---
        x: the action 
        y: the channel 
        """
        if y == 1:
            print("Slave 1 auto relock is not set up.")
            return

        await killall(y)
        # conversion from y to channel index
        self.plotter.channels[y-2] = 0

        for t in self.channel_task[y]:
            if not t.cancelled() and not t.done():
                t.cancel()
        try:
            if x == 1:
                # sweep
                self.channel_task[y].append(
                    asyncio.create_task(sweep(y, self.plotter.channels)))
            elif x == 2:
                # lock
                self.channel_task[y].append(asyncio.create_task(lock(y)))
        except Exception as e:
            print('gui', e)


async def main():
    # Create an instance of Tkinter frame or window
    win = MyTk()

    # Set the geometry of tkinter frame
    win.geometry("1000x500")

    # Set title of tkinter frame
    win.title('Slave Lock')

    # get default background colour
    defaultbg = win.cget('bg')

    # create labels for each row and column
    actions = ['SWEEP', 'LOCK', 'UNLOCK']

    for i in range(len(actions)):
        Label(win, text=actions[i], font=('Helvetica 15 bold')).grid(
            row=0, column=i+1, sticky=NSEW)

    for i in range(3):
        Label(win, text='Slave%s' % (i+1), font=('Helvetica 15 bold')
              ).grid(row=1+i, column=0, sticky=NSEW)

    Label(win, text='All', font=('Helvetica 15 bold')).grid(
        row=4, column=0, sticky=NSEW)

    def combine_funcs(*funcs):
        def inner_combined_func(*args, **kwargs):
            for f in funcs:
                f(*args, **kwargs)

        return inner_combined_func

    # create on-off button
    def create_btn(func):
        def switch():
            if(btn["text"] == "OFF"):
                btn.configure(text="ON", bg='green')

        btn = Button(win, text='OFF', font=font.Font(
            size=14), command=combine_funcs(switch, func))
        return btn

    button_dict = {}

    for i in range(1, 4):
        for j in range(1, 4):
            def test(x=j, y=i):
                asyncio.create_task(win.action(x, y))
                for k in range(1, 4):
                    if k != x:
                        if (button_dict[y*10+k]['text'] == 'ON'):
                            button_dict[y*10 +
                                        k].configure(text='OFF', bg=defaultbg)

            button_dict[i*10+j] = create_btn(test)
            button_dict[i*10+j].grid(row=i, column=j,
                                     sticky=NSEW, padx=1, pady=1)
            win.grid_columnconfigure(j, weight=1)
            win.grid_rowconfigure(i, weight=1)

    for i, l in zip(range(1, 4), actions):
        def test2(x=i):
            for j in range(1, 4):
                if button_dict[40+x]['text'] != button_dict[j*10+x]['text']:
                    button_dict[j*10+x].invoke()

        button_dict[40+i] = Button(win, text=l,
                                   font=font.Font(size=14), command=test2)
        button_dict[40+i].grid(row=4, column=i, sticky=NSEW, padx=1, pady=1)
        win.grid_columnconfigure(i, weight=1)
        win.grid_rowconfigure(4, weight=1)

    button_dict[43].invoke()
    await win.task[0]

    print("Cleaning up...")

asyncio.get_event_loop().run_until_complete(main())
