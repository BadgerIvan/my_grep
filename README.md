# MY_GREP
my_grep - small project for School 21 by Sber.

## Build
```bash
git clone [https://github.com/BadgerIvan/my_grep.git](https://github.com/BadgerIvan/my_grep.git)
cd my_grep/src
make
```

## Use
```bash
./my_grep [OPTION]... PATTERNS [FILE]...
```
Supported flags:
```
-e - pattern.
-i - ignore uppercase vs. lowercase.
-v - invert match.
-c - output count of matching lines only.
-l - output matching files only.
-n - precede each matching line with a line number.
-h - output matching lines without preceding them by file names.
-s - suppress error messages about nonexistent or unreadable files.
-f file - take regexes from a file.
-o - output the matched parts of a matching line.
```

## Run tests
```bash
make test
```