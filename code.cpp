#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <raylib.h>
#include <cmath>
#include <stdio.h>
#include <cstdint>

typedef int32_t i32;
typedef float f32;

constexpr i32 WIDTH = 800;
constexpr i32 HEIGHT = 600;
constexpr i32 MAX_ITERATIONS = 8;
constexpr i32 MAX_STRING_SIZE = 100000;
//C like L-System Fractal Tree

struct TurtleState {
    Vector2 position; 
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

static void StackFree(TurtleStack *stack) {
    free(stack->data);
}

static void StackPush(TurtleStack *stack, TurtleState &state) {
    if (stack->size >= stack->capacity) {
        stack->capacity *= 2;
        TurtleState *tmp = (TurtleState*)realloc(stack->data, stack->capacity * sizeof(TurtleState));
        if (!tmp) exit(EXIT_FAILURE);
        stack->data = tmp;
    }
    stack->data[stack->size++] = state;
}

static TurtleState StackPop(TurtleStack *stack) {
    return stack->data[--stack->size];
}

static char* ExpandLSystem(const char* axoim, i32 iterations, size_t maxSize) {
    size_t capacity = 1024;
    char* current = (char*)malloc(capacity);
    char *next = (char*)malloc(capacity);
    if (!current || !next) exit(EXIT_FAILURE);
    strcpy(current, axoim);
    for (i32 it = 0; it < iterations; ++it) {
        size_t length = std::strlen(current);
        size_t estimated = length * 11 + 1;
        if (estimated > maxSize) break;
        if (estimated > capacity) {
            capacity = estimated; 
            char* tmp1 = (char*)realloc(current, capacity); 
            char* tmp2 = (char*)realloc(next, capacity);
            if (!tmp1 || !tmp2) exit(EXIT_FAILURE);
            current = tmp1;
            next = tmp2;
        }
        size_t out = 0;
        for (size_t i = 0; i < length; ++i) {
            switch (current[i]) {
                case 'F':
                    next[out++] = 'F';
                    next[out++] = '[';
                    next[out++] = '+';
                    next[out++] = 'F';
                    next[out++] = ']';
                    next[out++] = 'F';
                    next[out++] = '[';
                    next[out++] = '-';
                    next[out++] = 'F';
                    next[out++] = ']';
                    next[out++] = 'F';
                    break;

                default:
                    next[out++] = current[i];
                    break;
            }
        }
        next[out] = '\0';
        char* tmp = current;
        current = next;
        next = tmp;
    }
    free(next);
    return current;
}

static void PlotLSystem(const char* system, Vector2 start, f32 initalAngle, f32 segmentLength, f32 angleStep) {
    TurtleStack stack;
    StackInit(&stack, 256);
    Vector2 position = start;
    f32 angle = initalAngle;
    for (const char* p = system; *p; ++p) {
        switch (*p) {
            case 'F': {
                f32 c = cosf(angle);
                f32 s = sinf(angle);
                Vector2 next = {position.x + c * segmentLength, position.y - s * segmentLength};
                DrawLineEx(position, next, 2.0f, BROWN);
                position = next;
                break;
            }
            case '+':
                angle += angleStep;
                break;

            case '-':
                angle -= angleStep;
                break;

            case '[':
            {
                TurtleState state = {
                    position,
                    angle
                };

                StackPush(&stack, state);
                break;
            }

            case ']':
            {
                TurtleState state = StackPop(&stack);

                position = state.position;
                angle = state.angle;
                break;
            }
        }
    }

    StackFree(&stack);
}

int main(void) {
    InitWindow( WIDTH, HEIGHT, "L-System Fractal Tree");
    SetTargetFPS(60);
    char *system = ExpandLSystem("F", 6, MAX_STRING_SIZE);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        PlotLSystem(system,(Vector2){WIDTH* 0.5f, HEIGHT- 30.0f },90.0f * DEG2RAD, 7.0f, 25.0f * DEG2RAD);
        DrawText("LSystem Fractal Tree",20,20,24,DARKGRAY);
        EndDrawing();
    }
    free(system);
    CloseWindow();
    return 0;
}
