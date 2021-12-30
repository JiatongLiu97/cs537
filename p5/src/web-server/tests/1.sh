timeout 1s ./server $TEST_PORT 8 -1 $TEST_SHM_NAME > /dev/null 2>&1
if [ $? == "124" ]; then
	echo "Fail: Expect the server to fail when [buffer] is invalid"
	exit 1
fi

timeout 1s ./server $TEST_PORT -1 16 $TEST_SHM_NAME > /dev/null 2>&1
if [ $? == "124" ]; then
	echo "Fail: Expect the server to fail when [threads] is invalid"
	exit 1
fi

timeout 1s ./server -1 8 16 $TEST_SHM_NAME > /dev/null 2>&1
if [ $? == "124" ]; then
	echo "Fail: Expect the server to fail when [port_num] is invalid"
	exit 1
fi

timeout 1s ./server 22 8 16 $TEST_SHM_NAME > /dev/null 2>&1
if [ $? == "124" ]; then
	echo "Fail: Expect the server to fail when [port_num] is in use"
	exit 1
fi

echo "Pass"
