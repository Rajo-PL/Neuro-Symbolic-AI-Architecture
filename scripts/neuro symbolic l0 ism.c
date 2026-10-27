#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#define L0_STATE_MIN -9
#define L0_STATE_MAX 9
#define HARD_STOP -999
typedef struct {
int8_t s_power;
int8_t s_temp;
int8_t s_flow;
} StateVector3D;
typedef struct {
bool is_sat;
int16_t action_code;
} GuardrailResult;
GuardrailResult evaluate_l0_guardrail(StateVector3D state) {
GuardrailResult res;
// Rule 1: Boundary check
if (abs(state.s_power) >= L0_STATE_MAX || abs(state.s_temp) >= L0_STATE_MAX) {
res.is_sat = false;
res.action_code = HARD_STOP;
return res;
}
// Rule 2: Conservation Law (Power + Temp == Flow)
if ((state.s_power + state.s_temp) != state.s_flow) {
res.is_sat = false;
res.action_code = HARD_STOP;
return res;
}
res.is_sat = true;
res.action_code = 0; // PERMIT
return res;
}
