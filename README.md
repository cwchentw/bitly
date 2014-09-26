# bitly

bitly - shorten URL with the power of bitly.com

## Usage

```
$ bitly http://www.google.com.tw
http://bit.ly/1tDK9YB
```

## Prerequisites

The program requires the following two library:

- libcurl
- json-c

Besides, because the program need the access token from bitly.com, you need to sign up a bitly.com account and verify your email to get the token.  See [this page](https://bitly.com/a/oauth_apps).

## Install

Install libcurl and json-c with system package manager on Linux.  On Mac, you may install libcurl and json-c with [Homebrew](http://brew.sh/).

After getting the access token from bitly.com, fill the token number into *token.h* or the program will become useless.

Just run `make` in the root directory.  Then, move the executive `bitly` to system executive directories like */usr/local/bin*.

## Description

The program query bitly.com for shortened URL.  Initially, I implemented the program in Perl.  However, there is a sensitive information in the program, the access token.  To prevent prying eye, I re-implement the program in C.

## Author

Michale Chen <cwchen123 AT gmail DOT com>

## Copyright

BSD license.
