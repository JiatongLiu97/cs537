from tester import Tester
import time
import sys

threads = int(sys.argv[1])
buffers = int(sys.argv[2])
num_client = int(sys.argv[3])
loops = int(sys.argv[4])
item_list = sys.argv[5:]

tester = Tester()
tester.run_server(threads=threads, buffers=buffers)

clients = [tester.run_client(item_list=item_list, loops=loops)
           for _ in range(num_client)]

stat_proc = tester.run_stat(threads=threads)

time.sleep(3)

for c in clients:
    c.start()
for c in clients:
    c.join()

time.sleep(10)
tester.kill_stat()
tester.kill_server()
