#pragma once

/*
    This file uses preprocessor #defines to set various Q_PROCESSOR_* #defines
    based on the following patterns:

    K_PROCESSOR_{FAMILY}
    K_PROCESSOR_{FAMILY}_{VARIANT}
    K_PROCESSOR_{FAMILY}_{REVISION}

    The first is always defined. Defines for the various revisions/variants are
    optional and usually dependent on how the compiler was invoked. Variants
    that are a superset of another should have a define for the superset.

    In addition to the processor family, variants, and revisions, we also set
    K_BYTE_ORDER appropriately for the target processor. For bi-endian
    processors, we try to auto-detect the byte order using the __BIG_ENDIAN__,
    __LITTLE_ENDIAN__, or __BYTE_ORDER__ preprocessor macros.

    Note: when adding support for new processors, be sure to update
    config.tests/arch/arch.cpp to ensure that configure can detect the target
    and host architectures.
*/

/* Machine byte-order, reuse preprocessor provided macros when available */
#if defined(__ORDER_BIG_ENDIAN__)
  #define K_BIG_ENDIAN __ORDER_BIG_ENDIAN__
#else
  #define K_BIG_ENDIAN 4321
#endif
#if defined(__ORDER_LITTLE_ENDIAN__)
  #define K_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#else
  #define K_LITTLE_ENDIAN 1234
#endif

/*
    Alpha family, no revisions or variants

    Alpha is bi-endian, use endianness auto-detection implemented below.
*/

#if defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(_M_ARM64) || defined(__aarch64__) || \
    defined(__ARM64__)
  #if defined(__aarch64__) || defined(__ARM64__) || defined(_M_ARM64)
    #define K_PROCESSOR_ARM_64
    #define K_PROCESSOR_WORDSIZE 8
  #else
    #define K_PROCESSOR_ARM_32
  #endif
  #if defined(__ARM_ARCH) && __ARM_ARCH > 1
    #define K_PROCESSOR_ARM __ARM_ARCH
  #elif defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM > 1
    #define K_PROCESSOR_ARM __TARGET_ARCH_ARM
  #elif defined(_M_ARM) && _M_ARM > 1
    #define K_PROCESSOR_ARM _M_ARM
  #elif defined(__ARM64_ARCH_8__) || defined(__aarch64__) || defined(__ARMv8__) || defined(__ARMv8_A__) || \
      defined(_M_ARM64)
    #define K_PROCESSOR_ARM 8
  #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || \
      defined(__ARM_ARCH_7S__) || defined(_ARM_ARCH_7) || defined(__CORE_CORTEXA__)
    #define K_PROCESSOR_ARM 7
  #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6T2__) || \
      defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6M__)
    #define K_PROCESSOR_ARM 6
  #elif defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_5TE__)
    #define K_PROCESSOR_ARM 5
  #else
    #define K_PROCESSOR_ARM 0
  #endif
  #if K_PROCESSOR_ARM >= 8
    #define K_PROCESSOR_ARM_V8
  #endif
  #if K_PROCESSOR_ARM >= 7
    #define K_PROCESSOR_ARM_V7
  #endif
  #if K_PROCESSOR_ARM >= 6
    #define K_PROCESSOR_ARM_V6
  #endif
  #if K_PROCESSOR_ARM >= 5
    #define K_PROCESSOR_ARM_V5
  #else
    #error "ARM architecture too old"
  #endif
  #if defined(__ARMEL__) || defined(_M_ARM64)
    #define K_BYTE_ORDER K_LITTLE_ENDIAN
  #elif defined(__ARMEB__)
    #define K_BYTE_ORDER K_BIG_ENDIAN
  #else
  // K_BYTE_ORDER not defined, use endianness auto-detection
  #endif

/*
    AVR32 family, no revisions or variants

    AVR32 is big-endian.
*/
// #elif defined(__avr32__)
// #  define K_PROCESSOR_AVR32
// #  define K_BYTE_ORDER Q_BIG_ENDIAN

/*
    Blackfin family, no revisions or variants

    Blackfin is little-endian.
*/
// #elif defined(__bfin__)
// #  define K_PROCESSOR_BLACKFIN
// #  define K_BYTE_ORDER K_LITTLE_ENDIAN

/*
    PA-RISC family, no revisions or variants

    PA-RISC is big-endian.
*/
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
  #define K_PROCESSOR_X86_32
  #define K_BYTE_ORDER         K_LITTLE_ENDIAN
  #define K_PROCESSOR_WORDSIZE 4

/*
 * We define K_PROCESSOR_X86 == 6 for anything above a equivalent or better
 * than a Pentium Pro (the processor whose architecture was called P6) or an
 * Athlon.
 *
 * All processors since the Pentium III and the Athlon 4 have SSE support, so
 * we use that to detect. That leaves the original Athlon, Pentium Pro and
 * Pentium II.
 */

  #if defined(_M_IX86)
    #define K_PROCESSOR_X86 (_M_IX86 / 100)
  #elif defined(__i686__) || defined(__athlon__) || defined(__SSE__) || defined(__pentiumpro__)
    #define K_PROCESSOR_X86 6
  #elif defined(__i586__) || defined(__k6__) || defined(__pentium__)
    #define K_PROCESSOR_X86 5
  #elif defined(__i486__) || defined(__80486__)
    #define K_PROCESSOR_X86 4
  #else
    #define K_PROCESSOR_X86 3
  #endif

#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
  #define K_PROCESSOR_X86 6
  #define K_PROCESSOR_X86_64
  #define K_BYTE_ORDER         K_LITTLE_ENDIAN
  #define K_PROCESSOR_WORDSIZE 8

/*
    MIPS family, known revisions: I, II, III, IV, 32, 64

    MIPS is bi-endian, use endianness auto-detection implemented below.
*/
#elif defined(__mips) || defined(__mips__) || defined(_M_MRX000)
  #define K_PROCESSOR_MIPS
  #if defined(_MIPS_ARCH_MIPS1) || (defined(__mips) && __mips - 0 >= 1)
    #define K_PROCESSOR_MIPS_I
  #endif
  #if defined(_MIPS_ARCH_MIPS2) || (defined(__mips) && __mips - 0 >= 2)
    #define K_PROCESSOR_MIPS_II
  #endif
  #if defined(_MIPS_ARCH_MIPS3) || (defined(__mips) && __mips - 0 >= 3)
    #define K_PROCESSOR_MIPS_III
  #endif
  #if defined(_MIPS_ARCH_MIPS4) || (defined(__mips) && __mips - 0 >= 4)
    #define K_PROCESSOR_MIPS_IV
  #endif
  #if defined(_MIPS_ARCH_MIPS5) || (defined(__mips) && __mips - 0 >= 5)
    #define K_PROCESSOR_MIPS_V
  #endif
  #if defined(_MIPS_ARCH_MIPS32) || defined(__mips32) || (defined(__mips) && __mips - 0 >= 32)
    #define K_PROCESSOR_MIPS_32
  #endif
  #if defined(_MIPS_ARCH_MIPS64) || defined(__mips64)
    #define K_PROCESSOR_MIPS_64
    #define K_PROCESSOR_WORDSIZE 8
  #endif
  #if defined(__MIPSEL__)
    #define K_BYTE_ORDER K_LITTLE_ENDIAN
  #elif defined(__MIPSEB__)
    #define K_BYTE_ORDER K_BIG_ENDIAN
  #else
  // K_BYTE_ORDER not defined, use endianness auto-detection
  #endif

/*
    Power family, known variants: 32- and 64-bit

    There are many more known variants/revisions that we do not handle/detect.
    See http://en.wikipedia.org/wiki/Power_Architecture
    and http://en.wikipedia.org/wiki/File:PowerISA-evolution.svg

    Power is bi-endian, use endianness auto-detection implemented below.
*/
#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) || defined(_ARCH_COM) || defined(_ARCH_PWR) || \
    defined(_ARCH_PPC) || defined(_M_MPPC) || defined(_M_PPC)
  #define K_PROCESSOR_POWER
  #if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
    #define K_PROCESSOR_POWER_64
    #define K_PROCESSOR_WORDSIZE 8
  #else
    #define K_PROCESSOR_POWER_32
  #endif
// K_BYTE_ORDER not defined, use endianness auto-detection

/*
    RISC-V family, known variants: 32- and 64-bit

    RISC-V is little-endian.
*/
#elif defined(__riscv)
  #define K_PROCESSOR_RISCV
  #if __riscv_xlen == 64
    #define K_PROCESSOR_RISCV_64
  #else
    #define K_PROCESSOR_RISCV_32
  #endif
  #define K_BYTE_ORDER K_LITTLE_ENDIAN

// -- Web Assembly --
#elif defined(__EMSCRIPTEN__)
  #define K_PROCESSOR_WASM
  #define K_BYTE_ORDER         K_LITTLE_ENDIAN
  #define K_PROCESSOR_WORDSIZE 8
  #ifdef KT_COMPILER_SUPPORTS_SSE2
    #define K_PROCESSOR_X86 6   // enables SIMD support
    #define K_PROCESSOR_X86_64  // wasm64
    #define K_PROCESSOR_WASM_64
  #endif

#endif

/*
  NOTE:
  GCC 4.6 added __BYTE_ORDER__, __ORDER_BIG_ENDIAN__, __ORDER_LITTLE_ENDIAN__
  and __ORDER_PDP_ENDIAN__ in SVN r165881. If you are using GCC 4.6 or newer,
  this code will properly detect your target byte order; if you are not, and
  the __LITTLE_ENDIAN__ or __BIG_ENDIAN__ macros are not defined, then this
  code will fail to detect the target byte order.
*/
// Some processors support either endian format, try to detect which we are using.
#if !defined(K_BYTE_ORDER)
  #if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == K_BIG_ENDIAN || __BYTE_ORDER__ == K_LITTLE_ENDIAN)
    // Reuse __BYTE_ORDER__ as-is, since our K_*_ENDIAN #defines match the preprocessor defaults
    #define K_BYTE_ORDER __BYTE_ORDER__
  #elif defined(__BIG_ENDIAN__) || defined(_big_endian__) || defined(_BIG_ENDIAN)
    #define K_BYTE_ORDER K_BIG_ENDIAN
  #elif defined(__LITTLE_ENDIAN__) || defined(_little_endian__) || defined(_LITTLE_ENDIAN)
    #define K_BYTE_ORDER K_LITTLE_ENDIAN
  #else
    #error "Unable to determine byte order!"
  #endif
#endif

/*
   Size of a pointer and the machine register size. We detect a 64-bit system by:
   * GCC and compatible compilers (Clang, ICC on OS X and Windows) always define
     __SIZEOF_POINTER__. This catches all known cases of ILP32 builds on 64-bit
     processors.
   * Most other Unix compilers define __LP64__ or _LP64 on 64-bit mode
     (Long and Pointer 64-bit)
   * If K_PROCESSOR_WORDSIZE was defined above, it's assumed to match the pointer
     size.
   Otherwise, we assume to be 32-bit and then check in qglobal.cpp that it is right.
*/

#if defined __SIZEOF_POINTER__
  #define K_POINTER_SIZE __SIZEOF_POINTER__
#elif defined(__LP64__) || defined(_LP64)
  #define K_POINTER_SIZE 8
#elif defined(K_PROCESSOR_WORDSIZE)
  #define K_POINTER_SIZE K_PROCESSOR_WORDSIZE
#else
  #define K_POINTER_SIZE 4
#endif

/*
   Define K_PROCESSOR_WORDSIZE to be the size of the machine's word (usually,
   the size of the register). On some architectures where a pointer could be
   smaller than the register, the macro is defined above.

   Falls back to QT_POINTER_SIZE if not set explicitly for the platform.
*/
#ifndef K_PROCESSOR_WORDSIZE
  #define K_PROCESSOR_WORDSIZE K_POINTER_SIZE
#endif
