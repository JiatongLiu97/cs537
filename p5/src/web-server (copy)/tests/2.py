from tester import Tester, diff, home_page_content

tester = Tester()
tester.run_server(threads=1, buffers=1)
response = tester.send_request_then_wait_response("/home.html")
msg = response.read().decode('utf-8')

if diff(msg, home_page_content):
    exit(1)

tester.kill_server()
print("Pass")
