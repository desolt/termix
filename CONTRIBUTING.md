# Contributing

## Code contribution

Please adhere to the style used in the rest of the repository:
* C99 (`--std=c99`)
* Allman style braces
* Pointer stars separated by a space on each side


You should run `make format` before sending a pull request (requires [`astyle`](http://astyle.sourceforge.net/)). See `.astyle` for more details. You can ensure that all your code is formatted nicely by adding a git pre-commit hook, placing the following in `.git/hooks/pre-commit`:
```sh
#!/bin/sh
make format
exit $?
```
You will then need to run `chmod +x .git/hooks/pre-commit` to give the file execute permission (otherwise git won't try and run it).
