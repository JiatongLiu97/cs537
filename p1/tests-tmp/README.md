# TO STUDENTS

This is the testing framework for project 1.

## Usage

To use this framework, first put your code onto a wisc CSL machine.

To list the set of public tests, please do:

```bash
/p/course/cs537-yuvraj/tests/p1/list-tests.sh
```

To run the tests, use the command at the path that contains your source files:

```bash
/p/course/cs537-yuvraj/tests/p1/run-tests.sh -c
```

To see details about the tester, use:

```bash
/p/course/cs537-yuvraj/tests/p1/tester/run-tests.sh -h
```


## Notes

There are public test cases for my-look, my-rev and pipe utilities. After running the command, you can see details of the test cases in `./test-tmp/name_of_the_utility/`. If any error occurs, follow the instructions printed to find out what is wrong with your output.

Filenames for the tests follow this convention:

testnum.desc: contains a short textual description of the purpose of the test
testnum.run: how to run this test
testnum.out: what your program should print to stdout
testnum.err: what your program should print to stderr
testnum.rc: the expected return code of your program
testnum.* : any other files are helper files (e.g., input files)

Passing all public cases does not guarantee full credit for this assignment. 20% of test cases are hidden, so make sure your code is robust so that you pass all the hidden test cases as well. These hidden test cases are not made to trick your code--they are straightforward.
