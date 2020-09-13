(lang dune 2.0)
(version 0.2.2)
(name shine)
(source (github savonet/ocaml-shine))
(license GPL-2.0)
(authors "Romain Beauxis <toots@rastageeks.org>")
(maintainers "The Savonet Team <savonet-users@lists.sourceforge.net>")

(generate_opam_files true)

(package
 (name shine)
 (synopsis "Fixed-point MP3 encoder")
 (description "shine is a super fast mp3 encoding library implemented in fixed-point arithmetic.")
 (depends
  (ocaml (>= 4.03.0))
  (dune (> 2.0)))
)
