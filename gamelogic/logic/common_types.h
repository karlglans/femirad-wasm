#ifndef COMMON_TYPEDEFS
#define COMMON_TYPEDEFS

typedef short Move;

// mainly for debugging
struct SelectTrace {
  Move move;
  int value;
  int depth;
};

#endif