module C = Configurator.V1

external is_big_endian : unit -> bool = "ocaml_lame_is_big_endian"

let () =
  C.main ~name:"shine-pkg-config" (fun c ->
      C.C_define.gen_header_file c ~fname:"config.h"
        [("IS_BIGENDIAN", Switch (is_big_endian ()))];

      let default : C.Pkg_config.package_conf =
        { libs = ["-lshine"]; cflags = [] }
      in
      let conf =
        match C.Pkg_config.get c with
          | None -> default
          | Some pc -> (
              match C.Pkg_config.query pc ~package:"shine" with
                | None -> default
                | Some deps -> deps)
      in

      C.Flags.write_sexp "c_flags.sexp" conf.cflags;
      C.Flags.write_sexp "c_library_flags.sexp" conf.libs)
