(*
 * Copyright 2003-2010 Savonet team
 *
 * This file is part of Ocaml-shine.
 *
 * Ocaml-shine is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Ocaml-shine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ocaml-shine; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *)

 (** OCaml bindings for the libshine. *)

type t

type parameters = 
  { 
    samplerate : int;
    channels   : int;
    bitrate    : int;
  }

exception Samples_per_frame
exception Invalid_bitrate
exception Invalid_samplerate
exception Invalid_channels

external samples_per_frame : unit -> int = "ocaml_shine_samples_per_frames"

let samples_per_frame = samples_per_frame()

external samplerate_index : int -> int = "ocaml_shine_samplerate_index"

external bitrate_index : int -> int = "ocaml_shine_bitrate_index"

external create : int -> int -> int -> t = "ocaml_shine_init"

let create params = 
  if samplerate_index params.samplerate < 0 then
    raise Invalid_samplerate;
  if bitrate_index params.bitrate < 0 then
    raise Invalid_bitrate;
  if params.channels < 1 || params.channels > 2 then
    raise Invalid_channels;
  create params.channels params.samplerate params.bitrate

external encode_buffer : t -> float array array -> int -> string = "ocaml_shine_encode_float"

let encode_buffer enc buf ofs = 
  if (Array.length buf == 0) || (Array.length buf - ofs != samples_per_frame) then
    raise Samples_per_frame;

  encode_buffer enc buf ofs

external encode_s16le : t -> string -> int -> string = "ocaml_shine_encode_s16le"

let encode_s16le enc data chans =
  if String.length data < 2*samples_per_frame*chans then
    raise Samples_per_frame;

  encode_s16le enc data chans
