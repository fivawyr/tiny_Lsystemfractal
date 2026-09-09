#include <cstddef>
#include <cstdlib>
#include <ranlib.h>
#include <cmath>
#include <stdio.h>
#include <cstdint>

typedef int32_t i32;
typedef float f32;

constexpr i32 WIDTH = 800;
constexpr i32 HEIGHT = 600;
constexpr i32 MAX_ITERATIONS = 8;

struct Vec2 {
    i32 x,y;
};

struct TurtleState {
    Vec2 position; 
    f32 angle;
};

struct TurtleStack {
    TurtleState *data;
    size_t size; 
    size_t capacity;
};

static void StackInit(TurtleStack *stack, size_t capacity) {
    stack->data = (TurtleState*)malloc(capacity * sizeof(TurtleState));
    stack->size = 0;
    stack->capacity = capacity;
}
