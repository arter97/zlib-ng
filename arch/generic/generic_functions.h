/* generic_functions.h -- generic C implementations for arch-specific functions.
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifndef GENERIC_FUNCTIONS_H_
#define GENERIC_FUNCTIONS_H_

#include "zendian.h"

Z_INTERNAL uint32_t crc32_fold_reset_c(crc32_fold *crc);
Z_INTERNAL void     crc32_fold_copy_c(crc32_fold *crc, uint8_t *dst, const uint8_t *src, size_t len);
Z_INTERNAL void     crc32_fold_c(crc32_fold *crc, const uint8_t *src, size_t len, uint32_t init_crc);
Z_INTERNAL uint32_t crc32_fold_final_c(crc32_fold *crc);

Z_INTERNAL uint32_t adler32_fold_copy_c(uint32_t adler, uint8_t *dst, const uint8_t *src, size_t len);


typedef uint32_t (*adler32_func)(uint32_t adler, const uint8_t *buf, size_t len);
typedef uint32_t (*compare256_func)(const uint8_t *src0, const uint8_t *src1);
typedef uint32_t (*crc32_func)(uint32_t crc32, const uint8_t *buf, size_t len);

uint32_t adler32_c(uint32_t adler, const uint8_t *buf, size_t len);

uint32_t chunksize_c(void);
uint8_t* chunkmemset_safe_c(uint8_t *out, uint8_t *from, unsigned len, unsigned left);
void     inflate_fast_c(PREFIX3(stream) *strm, uint32_t start);

uint32_t PREFIX(crc32_braid)(uint32_t crc, const uint8_t *buf, size_t len);

uint32_t compare256_c(const uint8_t *src0, const uint8_t *src1);
#if OPTIMAL_CMP >= 32
    uint32_t compare256_16(const uint8_t *src0, const uint8_t *src1);
#  ifdef HAVE_BUILTIN_CTZ
        uint32_t compare256_32(const uint8_t *src0, const uint8_t *src1);
#  endif
#  if defined(HAVE_BUILTIN_CTZLL) && OPTIMAL_CMP >= 64
        uint32_t compare256_64(const uint8_t *src0, const uint8_t *src1);
#  endif
#endif

typedef void (*slide_hash_func)(deflate_state *s);

void     slide_hash_c(deflate_state *s);

uint32_t longest_match_c(deflate_state *const s, Pos cur_match);
uint32_t longest_match_slow_c(deflate_state *const s, Pos cur_match);
#if OPTIMAL_CMP >= 32
    uint32_t longest_match_16(deflate_state *const s, Pos cur_match);
    uint32_t longest_match_slow_16(deflate_state *const s, Pos cur_match);
#  ifdef HAVE_BUILTIN_CTZ
        uint32_t longest_match_32(deflate_state *const s, Pos cur_match);
        uint32_t longest_match_slow_32(deflate_state *const s, Pos cur_match);
#  endif
#  if defined(HAVE_BUILTIN_CTZLL) && OPTIMAL_CMP >= 64
        uint32_t longest_match_64(deflate_state *const s, Pos cur_match);
        uint32_t longest_match_slow_64(deflate_state *const s, Pos cur_match);
#  endif
#endif


// Select generic implementation for longest_match, longest_match_slow, longest_match_slow functions.
#if OPTIMAL_CMP >= 32
#  if defined(HAVE_BUILTIN_CTZLL) && OPTIMAL_CMP >= 64
#    define longest_match_generic longest_match_64
#    define longest_match_slow_generic longest_match_slow_64
#    define compare256_generic compare256_64
#  elif defined(HAVE_BUILTIN_CTZ)
#    define longest_match_generic longest_match_32
#    define longest_match_slow_generic longest_match_slow_32
#    define compare256_generic compare256_32
#  else
#    define longest_match_generic longest_match_16
#    define longest_match_slow_generic longest_match_slow_16
#    define compare256_generic compare256_16
#  endif
#else
#  define longest_match_generic longest_match_c
#  define longest_match_slow_generic longest_match_slow_c
#  define compare256_generic compare256_c
#endif


#ifdef DISABLE_RUNTIME_CPU_DETECTION
// Generic code
#  define native_adler32 adler32_c
#  define native_adler32_fold_copy adler32_fold_copy_c
#  define native_chunkmemset_safe chunkmemset_safe_c
#  define native_chunksize chunksize_c
#  define native_crc32 PREFIX(crc32_braid)
#  define native_crc32_fold crc32_fold_c
#  define native_crc32_fold_copy crc32_fold_copy_c
#  define native_crc32_fold_final crc32_fold_final_c
#  define native_crc32_fold_reset crc32_fold_reset_c
#  define native_inflate_fast inflate_fast_c
#  define native_slide_hash slide_hash_c
#  define native_longest_match longest_match_generic
#  define native_longest_match_slow longest_match_slow_generic
#  define native_compare256 compare256_generic
#endif

#endif
