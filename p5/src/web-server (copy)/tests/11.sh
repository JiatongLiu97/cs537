#!/bin/bash

rm -f "/dev/shm/$TEST_SHM_NAME"

./server "$TEST_PORT" 1 1 "$TEST_SHM_NAME" > /dev/null 2>&1 &
server_pid="$!"

die() {
  kill -s SIGINT "$server_pid"
  exit 1
}

sleep 0.2

if [ ! -f "/dev/shm/$TEST_SHM_NAME" ]; then
  echo "Fail: Server does not create shared memory in /dev/shm"
  die
fi

timeout 1s ./stat_process > /dev/null 2>&1
if [ $? == "124" ]; then
	echo "Fail: Expect stat_process to fail without enough arguments"
  die
fi

timeout 1s ./stat_process "$TEST_SHM_NAME" -1 1 > /dev/null 2>&1
if [ $? == "124" ]; then
	echo "Fail: Expect stat_process to fail with invalid [sleeptime_ms]"
  die
fi

timeout 1s ./stat_process "$TEST_SHM_NAME" 1 -1 > /dev/null 2>&1
if [ $? == "124" ]; then
	echo "Fail: Expect stat_process to fail with invalid [num_threads]"
  die
fi

kill -s SIGINT "$server_pid"
sleep 0.2

if [ -f "/dev/shm/$TEST_SHM_NAME" ]; then
  echo "Fail: Server does not unlink shared memory on SIGINT"
  die
fi

timeout 1s ./stat_process "$TEST_SHM_NAME" 1 1 > /dev/null 2>&1
if [ $? == "124" ]; then
	echo "Fail: Expect stat_process to fail with non-existent shared memory segment"
  die
fi

echo "Pass"
