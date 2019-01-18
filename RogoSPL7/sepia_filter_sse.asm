global sepia_filter_sse

section .rodata

; 1 float pixel component -  4 bytes, xmm = 16 bytes
; 16 - four pixel components
; 4 channels parallel
; r g b r
; g b r g
; b r g b

; create matrix for filter

;b g r b
align 16
blue_channel_bgrb: dd 0.131, 0.168, 0.189, 0.131
align 16
green_channel_bgrb: dd 0.543, 0.686, 0.769, 0.543
align 16
red_channel_bgrb: dd 0.272, 0.349, 0.393, 0.272

; g r b g
align 16
blue_channel_grbg: dd 0.168, 0.189, 0.131, 0.168
align 16
green_channel_grbg: dd 0.686, 0.769, 0.543, 0.686
align 16
red_channel_grbg: dd 0.349, 0.393, 0.272, 0.349

;r b g r
align 16
blue_channel_rbgr: dd 0.189, 0.131, 0.168, 0.189
align 16
green_channel_rbgr: dd 0.769, 0.543, 0.686, 0.769
align 16
red_channel_rbgr: dd 0.393, 0.272, 0.349, 0.393

section .text

; define necessary registers

; pixel pointer
%define pointer rcx

; matrix data
%define xmm_blue_channel_bgrb xmm3
%define xmm_green_channel_bgrb xmm4
%define xmm_red_channel_bgrb xmm5

%define xmm_blue_channel_grbg xmm6
%define xmm_green_channel_grbg xmm7
%define xmm_red_channel_grbg xmm8

%define xmm_blue_channel_rbgr xmm9
%define xmm_green_channel_rbgr xmm10
%define xmm_red_channel_rbgr xmm11

; color channels

%define channel_blue xmm0
%define channel_green xmm1
%define channel_red xmm2

; color accs

%define xmm_bgrb xmm12
%define xmm_grbg xmm13
%define xmm_rbgr xmm14


sepia_filter_sse:

; rdi - pointer to the pixel array
; rsi - number of pixels

  mov pointer, rdi
  lea rsi, [rsi+2*rsi] ; bytes to process - pixel size is 3 bytes
  add rsi, rdi  ; rsi - address after pixel bytes

; save matrixes in xmm, store columns
; movaps - send packed data aligned 16

  movaps xmm_blue_channel_bgrb, [blue_channel_bgrb]
  movaps xmm_green_channel_bgrb, [green_channel_bgrb]
  movaps xmm_red_channel_bgrb, [red_channel_bgrb]
 
  movaps xmm_blue_channel_grbg, [blue_channel_grbg]
  movaps xmm_green_channel_grbg, [green_channel_grbg]
  movaps xmm_red_channel_grbg, [red_channel_grbg]
 
  movaps xmm_blue_channel_rbgr, [blue_channel_rbgr]
  movaps xmm_green_channel_rbgr, [green_channel_rbgr]
  movaps xmm_red_channel_rbgr, [red_channel_rbgr]

sepia_filter_sse_loop:

  mov rdx, [pointer]      ; get first pixel

  ; b g r b

  movd channel_blue, edx ; get  b g r b g r b g in bytes
  pmovzxbd channel_blue, channel_blue ;  b0 g0 r0 b1 in 4 bytes - convert byte to dword + zx - zero extended
  cvtdq2ps channel_blue, channel_blue ; Convert Packed Doubleword Integers to Packed Single-Precision Floating-Point Values
  shufps channel_blue, channel_blue, 0b11000000 ; Packed Interleave Shuffle of Quadruplets of Single-Precision Floating-Point Values
  ; b0 b0 b0 b1

  shr rdx, 8                ; mov to g r b .. 00 - 8 bit - size of pixel component
  movd channel_green, edx
  pmovzxbd channel_green, channel_green ; g0 r0 b1 g1
  cvtdq2ps channel_green, channel_green
  shufps channel_green, channel_green, 0b11000000
  ; g0 g0 g0 g1

  shr rdx, 8                ; mov to r b .. 00 00
  movd channel_red, edx
  pmovzxbd channel_red, channel_red ; r0 b1 g1 r1
  cvtdq2ps channel_red, channel_red
  shufps channel_red, channel_red, 0b11000000
  ; r0 r0 r0 r1

  xorps xmm_bgrb, xmm_bgrb
  ; Fused Multiply-Add of Packed SinglePrecision Floating-Point Values
  vfmadd231ps xmm_bgrb, channel_blue, xmm_blue_channel_bgrb ; xmm_bgrb += channel_blue * xmm_blue_channel_bgrb
  vfmadd231ps xmm_bgrb, channel_green, xmm_green_channel_bgrb ; xmm_bgrb += channel_green * xmm_green_channel_bgrb
  vfmadd231ps xmm_bgrb, channel_red, xmm_red_channel_bgrb ; xmm_bgrb += channel_red * xmm_red_channel_bgrb
; necessary b0 g0 r0 b1

; g r b g

  shr rdx, 8                ; b g r b g 00 00 00
  movd channel_blue, edx
  pmovzxbd channel_blue, channel_blue ; b1 g1 r1 b2
  cvtdq2ps channel_blue, channel_blue
  shufps channel_blue, channel_blue, 0b11110000
; b1 b1 b2 b2

  mov rdx, [pointer + 4]  ; g r b g r b g r
  movd channel_green, edx
  pmovzxbd channel_green, channel_green ; g1 r1 b2 g2
  cvtdq2ps channel_green, channel_green
  shufps channel_green, channel_green, 0b11110000
; g1 g1 g2 g2

  shr rdx, 8                ; r b g r b g r 00
  movd channel_red, edx
  pmovzxbd channel_red, channel_red  ; r1 b2 g2 r2
  cvtdq2ps channel_red, channel_red
  shufps channel_red, channel_red, 0b11110000
; r1 r1 r2 r2
  
  xorps xmm_grbg, xmm_grbg
  vfmadd231ps xmm_grbg, channel_blue, xmm_blue_channel_grbg
  vfmadd231ps xmm_grbg, channel_green, xmm_green_channel_grbg
  vfmadd231ps xmm_grbg, channel_red, xmm_red_channel_grbg

  ; r b g r

  shr rdx, 8                ; b g r b g r 00 00 ..
  movd channel_blue, edx
  pmovzxbd channel_blue, channel_blue ; b2 g2 r2 b3
  cvtdq2ps channel_blue, channel_blue
  shufps channel_blue, channel_blue, 0b11111100
 ; b2 b3 b3 b3

  shr rdx, 8                ; g r b g r 00 00 ..
  movd channel_green, edx
  pmovzxbd channel_green, channel_green ; g2 r2 b3 g3
  cvtdq2ps channel_green, channel_green
  shufps channel_green, channel_green, 0b11111100
  ; g2 g3 g3 g3

  shr rdx, 8                ;  r b g r 00 ..
  movd channel_red, edx
  pmovzxbd channel_red, channel_red ; r2 b3 g3 r3
  cvtdq2ps channel_red, channel_red
  shufps channel_red, channel_red, 0b11111100
 ; r2 r3 r3 r3

  xorps xmm_rbgr, xmm_rbgr
  vfmadd231ps xmm_rbgr, channel_blue, xmm_blue_channel_rbgr
  vfmadd231ps xmm_rbgr, channel_green, xmm_green_channel_rbgr
  vfmadd231ps xmm_rbgr, channel_red, xmm_red_channel_rbgr

  ; getting results
; Convert Packed Single-Precision Floating-Point Values to Packed Signed Doubleword Integer Values - convertion
  ; float to int32
  cvtps2dq xmm_bgrb, xmm_bgrb
  cvtps2dq xmm_grbg, xmm_grbg
  cvtps2dq xmm_rbgr, xmm_rbgr

  ; Pack with Signed Saturation
  ; int32 to int16
  packssdw xmm_bgrb, xmm_grbg ; r0 g0 b0 r1 g1 b1 r2 g2 - in bgrb
  ;  b2 r3 g3 b3  b2 r3 g3 b3 - in grbg
  packssdw xmm_rbgr, xmm_rbgr
  ; int16  to int8
  packuswb xmm_bgrb, xmm_rbgr
  ; bgrb - r0 g0 b0 r1 g1 b1 r2 g2 b2 r3 g3 b3  b2 r3 g3 b3

  ; write pixels (12 bytes) back to pointer
  pextrq [pointer], xmm_bgrb, 0  ; Extract Qword - write 8 bytes (half) r0 g0 b0 r1 g1 b1 r2 g2
  pextrd [pointer + 8], xmm_bgrb, 2  ; Extract Dword - third quater b2 r3 g3 b3
  lea pointer, [pointer + 12] ; next part
  cmp pointer, rsi
  jl sepia_filter_sse_loop ;check if not end

  ret