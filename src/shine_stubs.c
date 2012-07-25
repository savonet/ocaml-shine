/*
 * OCaml bindings for libshine
 *
 * Copyright 2005-2010 Savonet team
 *
 * This file is part of ocaml-shine.
 *
 * ocaml-shine is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ocaml-shine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ocaml-shine; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

 /* OCaml bindings for the libshine library. */

#include <caml/custom.h>
#include <caml/signals.h>
#include <caml/misc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/alloc.h>

#include <shine/layer3.h>

#include <string.h>

#include "config.h"

#define Encoder_val(v) (*((shine_t**)Data_custom_val(v)))

static void finalize_encoder(value e)
{
  L3_close(Encoder_val(e));
}

static struct custom_operations encoder_ops =
{
  "ocaml_shine_encoder",
  finalize_encoder,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

CAMLprim value ocaml_shine_samples_per_frames(value unit)
{
  CAMLparam0();
  CAMLreturn(Val_int(samp_per_frame));
}

CAMLprim value ocaml_shine_bitrate_index(value br)
{
  CAMLparam0();
  CAMLreturn(Val_int(L3_find_bitrate_index(Int_val(br))));
}

CAMLprim value ocaml_shine_samplerate_index(value sr)
{
  CAMLparam0();
  CAMLreturn(Val_int(L3_find_samplerate_index(Int_val(sr))));
}

CAMLprim value ocaml_shine_init(value chans, value samplerate, value bitrate)
{
  CAMLparam0();
  CAMLlocal1(ans);
  shine_config_t config;
  shine_t *enc;

  L3_set_config_mpeg_defaults(&config.mpeg);
  
  config.wave.channels   = Int_val(chans);
  config.wave.samplerate = Int_val(samplerate); 
  config.mpeg.bitr       = Int_val(bitrate);
  if (config.wave.channels == 1)
    config.mpeg.mode = 3;
  else
    config.mpeg.mode == 1;

  enc = L3_initialise(&config);

  ans = caml_alloc_custom(&encoder_ops, sizeof(shine_t *), 1, 0);
  Encoder_val(ans) = enc;   

  CAMLreturn(ans);
}

static inline int16_t clip(double s)
{
  if (s < -1)
  {
    return INT16_MIN;
  }
  else if (s > 1)
  {
    return INT16_MAX;
  }
  else
    return (s * INT16_MAX);
}

static inline int16_t bswap_16 (int16_t x) { return ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)); }

CAMLprim value ocaml_shine_encode_float(value e, value data)
{
  CAMLparam2(e,data);
  CAMLlocal2(src,ret);
  int16_t pcm[2][samp_per_frame];
  int c,i;
  long written;
  unsigned char *outdata;

  shine_t *enc = Encoder_val(e);

  for (c = 0; c < Wosize_val(data); c++)
  {
    src = Field(data, c);
    for (i = 0; i < samp_per_frame; i++)
    {
      pcm[c][i] = clip(Double_field(src, i));
#ifdef BIGENDIAN
      pcm[c][i] = bswap_16(pcm[c][i]);
#endif
    }
  }

  caml_enter_blocking_section();

  outdata = L3_encode_frame(enc, pcm, &written);

  caml_leave_blocking_section();

  ret = caml_alloc_string(written);
  memcpy(String_val(ret),outdata,written);
   
  CAMLreturn(ret);
}

CAMLprim value ocaml_shine_encode_s16le(value e, value data, value channels)
{
  CAMLparam2(e,data);
  CAMLlocal1(ret);
  int16_t pcm[2][samp_per_frame];
  int16_t *src = (int16_t *)String_val(data);
  int c,i;
  long written;
  int chans = Int_val(channels);
  
  unsigned char *outdata;

  shine_t *enc = Encoder_val(e);

  for (c = 0; c < chans; c++)
  {
    for (i = 0; i < samp_per_frame; i++)
    {
      pcm[c][i] = src[i*chans + c];
#ifdef BIGENDIAN
      pcm[c][i] = bswap_16(pcm[c][i]);
#endif
    }
   }

   caml_enter_blocking_section();

   outdata = L3_encode_frame(enc, pcm, &written);

   caml_leave_blocking_section();

   ret = caml_alloc_string(written);
   memcpy(String_val(ret),outdata,written);

  CAMLreturn(ret);
}
