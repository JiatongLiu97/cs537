import time
import subprocess
from tester import Tester

tester = Tester()
server_proc = tester.run_server(threads=4, buffers=8)
time.sleep(5)

args = ["ps", "--no-headers", "-o", "time", f"{server_proc.pid}"]
ps = subprocess.run(args, stdout=subprocess.PIPE, text=True)

t_hr, t_min, t_sec = tuple(map(int, ps.stdout.split(':')))
t_total = t_sec + (60 * (t_min + (60 * t_hr)))

if t_total != 0:
    print(f"Fail: idle server spent {t_total} seconds running on CPU, which should be 0")
    tester.kill_server()
    exit(1)

tester.kill_server()
print("Pass")
