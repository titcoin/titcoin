Titcoin Wallet
=====================================
[![Build Status](https://travis-ci.com/titcoin/titcoin.svg?branch=master)](https://travis-ci.com/titcoin/titcoin)

About
-----

This is the current Titcoin wallet made by Gandalf86. For more info about Titcoin, see https://bitcointalk.org/index.php?topic=5018869.0

Titcoin is based on [Bitcoin Core](https://github.com/bitcoin/bitcoin) version 0.16.3, all credits go to the Bitcoin Core developers. Most documentation files have been left untouched to avoid confusion.

License
-------

Bitcoin Core and Titcoin are released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Getting the source code
------------------------

If you are not a Titcoin developer and are only downloading the source code in order to deploy the software (i.e. installing it on your system), please always download a **fresh copy** of the source tree! Use this link for downloading the source code: https://github.com/titcoin/titcoin/archive/master.zip Or even better, go to https://github.com/titcoin/titcoin/releases and download the source code that is attached to the release that you want to install on your system.

Do not use `git pull` or `git fetch` for downloading updates since the last checkout! The reason is that we might do coordinated rebases from time to time for cleaning up commits, and you will get confusing error messages when attempting to pull a rebased branch into your local branch. If you are a developer interested in getting involved, please get in contact so we can work out a coordinated development strategy.

Building
--------

Please refer to the build notes in the [doc/](doc/) directory for compiling the wallet.

Testing
-------

* Unit tests can be run with `make check`
* Regression tests can be run with `./test/functional/test_runner.py`

All unit tests should pass. Please report if there are any errors and only use the wallet if all included tests pass. For more info, see [/src/test/README.md](/src/test/README.md).
