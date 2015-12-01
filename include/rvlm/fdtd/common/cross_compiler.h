#ifndef RFDTD_CROSS_COMPILER_H
#define RFDTD_CROSS_COMPILER_H

#ifdef __GNUC__
#define RFDTD_NORETURN __attribute__((__noreturn__))
#else
#define RFDTD_NORETURN
#endif

#endif
