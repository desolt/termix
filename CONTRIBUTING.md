# Contributing

## Code contribution

Please adhere to the style used in the rest of the repository:
* C99 (`--std=c99`)
* Allman style braces
* Pointer stars separated by a space on each side


You should run `make format` before sending a pull request (requires [`astyle`](http://astyle.sourceforge.net/)). See `.astyle` for more details. You can ensure that all your code is formatted nicely and tests pass by adding a git pre-commit hook, placing the following in `.git/hooks/pre-commit`:
```
#!/bin/sh
make format || exit 1
make test || exit 1
```
You will then need to run `chmod +x .git/hooks/pre-commit` to give the file execute permission (otherwise git won't try and run it).

### Adding tests

Tests are contained in the `tests/` folder. The test framework used for this project is a slightly modified version of [minunit](https://github.com/siu/minunit). One important modification that we have made is to ensure that each test executable returns 0 from main if all checks passed, and non-zero otherwise. `MU_REPORT` has been modified such that it calls return on its own. You can find an example test (which you should use as a base) in `tests/test_example.c`. Otherwise, you should refer to the `tests/minunit.h` header file or to the [minunit GitHub page](https://github.com/siu/minunit).
