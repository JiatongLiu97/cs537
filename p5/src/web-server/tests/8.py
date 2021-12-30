from tester import Tester

threads = 1
buffers = 30
num_client = 25
loops = 20
item_list = ["/home.html", "/favicon.ico"]

tester = Tester()
tester.run_server(threads=threads, buffers=buffers)

clients = [tester.run_client(item_list=item_list, loops=loops)
           for _ in range(num_client)]

for c in clients:
    c.start()
for c in clients:
    c.join()

tester.kill_server()
print("Pass")
