ocaml-shine
===========

This package contains an OCaml interface for the shine fixed-point encoding
library. Please read the COPYING file before using this software.

Prerequisites
-------------

- ocaml >= 3.10.2 (haven't tried earlier versions)
- libshine >= 0.1.0
- findlib >= 0.8.1 (haven't tried earlier versions)

Compilation
-----------

```sh
make all
```

This should build both the native and the byte-code version of the extension
library.

Installation
------------

```sh
make install
```

This should install the library file (using ocamlfind) in the appropriate place.

Author
------

This author of this software may be contacted by electronic mail at the
following address: <savonet-users@lists.sourceforge.net>.
