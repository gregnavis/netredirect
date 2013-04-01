netredirect - network traffic redirection utility
=================================================

[![Build Status](https://travis-ci.org/grn/netredirect.png?branch=master)](https://travis-ci.org/grn/netredirect)

`netredirect` allows you to redirect *all* outbound connections made
from a given program to another host. For example:

```
netredirect 127.0.0.1 8080 curl http://www.google.com
```

will send the request to port 8080 on `localhost`!

Because of the method that `netredirect` uses to achieve its result only
programs that use network system calls through `libc` wrappers can be
redirected.

When the host or port argument to `netredirect` is `original` then the
original value is preserved. That allows you to perform host-only and
port-only redirection.

Installation
------------

In order to install do the following:

```
git clone https://github.com/grn/netredirect
cd netredirect
make all
```

Then as root:

```
make install
```

The files are installed under `/usr/bin` and `/usr/lib` by default. The
locations can be changed by passing `BIN_PATH` and `LIB_PATH` variables
to `make`. If you want to install under `/bin` and `/lib` do the following:

```
make BIN_PATH=/bin LIB_PATH=/lib install
```

Now you are ready to use it!

Uninstallation is left as an exercise to the user.

Motivation
----------

The original motivation for this program was load/stress testing of a
service that used lots of external services. In order to test its
production configuration without generating significant load on these
services a redirection method was necessary. `netredirect` allows to
perform such tests with ease!

Internals
---------

The tool is extremely simple. The best way to understand it is to look
at the source code. Basically it uses `LD_PRELOAD` trickery to wrap the
`connect` function of `libc` with its own. The target IP address is
stored in an environment variable that our library accesses. The
environment is prepared by the `netredirect` script.

Limitations
-----------

  - currently `netredirect` and `libnetredirect.so` *must* be in the
    same directory
  - currently only TCP is supported
  - may break DNS name resolution if done over TCP
  - `NETREDIRECT_HOST` *must* be an IPv4 address in the dotted notation
  - no UDP or IPv6 support
  - no error checking in the `netredirect` script
