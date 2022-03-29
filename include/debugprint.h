/*
 * Debug level output macros
 */
#ifndef DEBUGPRINT_H
#define DEBUGPRINT_H

#define DEBUG true
#define debug_printf(frmt, ...)                                                                                                            \
  do {                                                                                                                                     \
    if (DEBUG)                                                                                                                             \
      console << fmt::sprintf("%-32s:%3d %-36s: " frmt, __FILE__, __LINE__, __func__, __VA_ARGS__);                                        \
  } while (0)

#define DEBUG2 false
#define debug_printf_l2(frmt, ...)                                                                                                         \
  do {                                                                                                                                     \
    if (DEBUG2)                                                                                                                            \
      console << fmt::sprintf("%-32s:%3d %-36s: " frmt, __FILE__, __LINE__, __func__, __VA_ARGS__);                                        \
  } while (0)

#define DEBUG3 false
#define debug_printf_l3(frmt, ...)                                                                                                         \
  do {                                                                                                                                     \
    if (DEBUG3)                                                                                                                            \
      console << fmt::sprintf("%-32s:%3d %-36s: " frmt, __FILE__, __LINE__, __func__, __VA_ARGS__);                                        \
  } while (0)

#endif // DEBUGPRINT_H
