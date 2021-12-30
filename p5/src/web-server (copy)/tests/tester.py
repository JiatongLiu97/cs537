import subprocess
import http.client
import time
import signal
import os
import threading
from typing import Tuple, List

home_page_content = '<html>\n\n'\
    '<head>\n' \
    '  <title>537 Test Web Page</title>\n' \
    '</head>\n\n' \
    '<body>\n\n' \
    '<h2> 537 Test Web Page</h2>\n\n' \
    '<p> Test web page: simply awesome.</p>\n\n' \
    '<p>\n' \
    '    Click <a href="https://www.youtube.com/watch?v=dQw4w9WgXcQ"> here</a> for something\n' \
    'even more awesome.\n' \
    '</p>\n\n' \
    '</body>\n' \
    '</html>\n'


# Raise this exception in the case of test case failure
class TestFailure(Exception):
    def __init__(self, reason):
        self.reason = reason

    def __str__(self) -> str:
        return f"{self.reason}"


# Return true if has difference
def diff(get: str, expect: str) -> bool:
    if get != expect:
        print("Incorrect content fetched!")
        print("=== Expect ===")
        print(expect)
        print("=== Get ===")
        print(get)
        return True
    return False


class Tester:
    def __init__(self) -> None:
        self.port = int(os.environ["TEST_PORT"])
        self.shm_name = os.environ["TEST_SHM_NAME"]
        self.debug_mode = True if os.environ["TEST_DEBUG_MODE"] == "1" else False
        self.server_proc = None
        self.stat_proc = None

    def run_server(self, threads: int, buffers: int) -> subprocess.Popen:
        # Run server; expect the server not fail
        if self.debug_mode:
            self.server_proc = subprocess.Popen(["./server", f"{self.port}", f"{threads}", f"{buffers}", self.shm_name])
        else:
            self.server_proc = subprocess.Popen(["./server", f"{self.port}", f"{threads}", f"{buffers}", self.shm_name], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(1)  # give the server some time to start
        if not self.isServerAlive():  # the server process exits
            raise TestFailure(
                f"Server exits unexpectedly with return code {self.server_proc.returncode}")
        return self.server_proc

    def kill_server(self) -> None:
        if not self.isServerAlive():
            raise TestFailure(
                f"Server exits unexpectedly with return code {self.server_proc.returncode}")
        self.server_proc.send_signal(signal.SIGINT)

    def run_stat(self, threads: int) -> subprocess.Popen:
        self.stat_proc = subprocess.Popen(["./stat_process", self.shm_name, "50", f"{threads}"])
        return self.stat_proc

    def kill_stat(self) -> None:
        self.stat_proc.send_signal(signal.SIGINT)

    def isServerAlive(self) -> bool:
        if self.server_proc is None:
            return False
        return self.server_proc.poll() == None

    def send_request(self, item: str, timeout=10) -> http.client.HTTPSConnection:
        conn = http.client.HTTPConnection(
            "localhost", self.port, timeout=timeout)
        try:
            conn.request('GET', item)
        except Exception as e:
            raise TestFailure("Unable to send request to the server")
        return conn

    def wait_response(self, conn: http.client.HTTPSConnection) -> http.client.HTTPResponse:
        try:
            response = conn.getresponse()
            conn.close()
        except Exception:
            raise TestFailure("Unable to get response from the server")
        return response

    def send_request_then_wait_response(self, item: str) -> http.client.HTTPResponse:
        conn = self.send_request(item=item)
        return self.wait_response(conn=conn)

    def run_client_workload(self, item_list: List[str], loops: int) -> None:
        for _ in range(loops):
            for item in item_list:
                self.send_request_then_wait_response(item)

    def run_client(self, item_list: List[str], loops: int) -> threading.Thread:
        # create a client thread
        return threading.Thread(target=self.run_client_workload, args=(item_list, loops))
