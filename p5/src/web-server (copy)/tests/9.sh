#!/bin/bash

rm -f "/dev/shm/$TEST_SHM_NAME"

./server "$TEST_PORT" 1 1 "$TEST_SHM_NAME" > /dev/null 2>&1 &
server_pid="$!"

die() {
  kill -s SIGINT "$server_pid"
  exit 1
}

sleep 0.2

if [ -f "/dev/shm/$TEST_SHM_NAME" ]; then
  echo "Pass: shared memory successfully created in /dev/shm"
else
  echo "Fail: shared memory not created in /dev/shm"
  die
fi

shm_size="$(ls -l "/dev/shm/$TEST_SHM_NAME" | awk '{print $5}')"
page_size="$(getconf PAGESIZE)"
if [ "$shm_size" == "$page_size" ]; then
  echo "Pass: shared memory is the correct size"
else
  echo "Fail: shared memory is the wrong size"
  echo "Shared memory size: $shm_size"
  echo "Page size: $page_size"
  die
fi

kill -s SIGINT "$server_pid"
