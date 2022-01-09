# bitly

`bitly` - shorten URLs with the power of bitly.com

## Usage

```shell
$ bitly https://google.com.tw
https://bit.ly/3tf60Kv
```

## Prerequisites

* libcurl
* json-c

In addition, the program needs an access token from bitly.com. You need to sign up a bitly.com account and verify your email to get a token.  See [this page](https://bitly.com/a/oauth_apps) for more.

## Install

Install libcurl and json-c with system package manager on Linux.  On Mac, you may install libcurl and json-c with [Homebrew](http://brew.sh/).

Just run `make` in the root directory of the project.  Then, move the executive `bitly` to a system executive directory like */usr/local/bin*.

## Breaking Changes

* 2022/01/09
  * Load Bitly token from `BITLY_TOKEN` the environment variable

## Copyright

Copyright (c) 2022 Michelle Chen. Licensed under BSD
