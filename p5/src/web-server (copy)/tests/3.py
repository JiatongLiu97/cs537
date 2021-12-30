import time
from tester import Tester

tester = Tester()
tester.run_server(threads=3, buffers=8)

conns = []
for _ in range(6):
    conns.append(tester.send_request("/output.cgi?1"))
    time.sleep(0.1)  # ensure ordering

for i in range(0, 3):
    tester.wait_response(conns[i])
t1 = time.time()

for i in range(3, 6):
    tester.wait_response(conns[i])
t2 = time.time()

# Debug Hint from TA:
# If you are using a fixed-size thread pool, the first three requests should be
# served by the three worker threads first. "output.cgi" takes an argument to
# specify how long this worker thread should block. In this case, it is one
# second. So the last three requests won't be served until one second later
# (after the first three requests done).
if t2 < t1 + 1:
    print("Not using a fixed-size thread pool (size 3)")
    exit(1)

tester.kill_server()
print("Pass")
