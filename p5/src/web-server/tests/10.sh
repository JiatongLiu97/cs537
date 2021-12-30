#!/bin/bash

rm -f "/dev/shm/$TEST_SHM_NAME"

./server "$TEST_PORT" 1 1 "$TEST_SHM_NAME" > /dev/null 2>&1 &
server_pid="$!"

sleep 0.2

if [ -f "/dev/shm/$TEST_SHM_NAME" ]; then
  echo "Pass: shared memory successfully created in /dev/shm"
else
  echo "Fail: shared memory not created in /dev/shm"
fi

kill -s SIGINT "$server_pid"
sleep 0.2

if [ -f "/dev/shm/$TEST_SHM_NAME" ]; then
  echo "Fail: shared memory not unlinked"
else
  echo "Pass: shared memory successfully unlinked"
fi
