/*
 * Debug level output macros
 */
#ifndef DEBUGPRINT_H
#define DEBUGPRINT_H

extern bool debug;
extern bool debugl2;
extern bool debugl3;

#define debug_printf(frmt, ...)                                                                                                            \
  do {                                                                                                                                     \
    if (debug)                                                                                                                             \
      console << fmt::sprintf("%-32s:%3d %-36s: " frmt, __FILE__, __LINE__, __func__, __VA_ARGS__);                                        \
  } while (0)

#define debug_printf_l2(frmt, ...)                                                                                                         \
  do {                                                                                                                                     \
    if (debugl2)                                                                                                                           \
      console << fmt::sprintf("%-32s:%3d %-36s: " frmt, __FILE__, __LINE__, __func__, __VA_ARGS__);                                        \
  } while (0)

#define debug_printf_l3(frmt, ...)                                                                                                         \
  do {                                                                                                                                     \
    if (debugl3)                                                                                                                           \
      console << fmt::sprintf("%-32s:%3d %-36s: " frmt, __FILE__, __LINE__, __func__, __VA_ARGS__);                                        \
  } while (0)

#endif // DEBUGPRINT_H
