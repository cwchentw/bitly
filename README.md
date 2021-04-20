# bitly

`bitly` - shorten URLs with the power of bitly.com

## Usage

```
$ bitly http://www.google.com.tw
http://bit.ly/1tDK9YB
```

## Prerequisites

The program requires the following two libraries:

- libcurl
- json-c

In addition, the program needs an access token from bitly.com. You need to sign up a bitly.com account and verify your email to get a token.  See [this page](https://bitly.com/a/oauth_apps) for more.

## Install

Install libcurl and json-c with system package manager on Linux.  On Mac, you may install libcurl and json-c with [Homebrew](http://brew.sh/).

After getting an access token from bitly.com, fill the token number into our header *token.h* or the program will become useless.

Just run `make` in the root directory of the project.  Then, move the executive `bitly` to a system executive directory like */usr/local/bin*.

## Description

The program query bitly.com to get a shortened URL.  Initially, I implemented the program in Perl.  However, there is a sensitive information in the program, an access token.  To prevent prying eye, I re-implement the program in C.

If your `bitly` executable is stolen, you can revoke the old token and apply for a new one.

## Author

Michale Chen <cwchen123 AT gmail DOT com>

## Copyright

BSD license.
