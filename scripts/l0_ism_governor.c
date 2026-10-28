/**
 * @file l0_ism_governor.c
 * @brief Test Driver for C99 RTOS L0-ISM Real-Time Governor
 */

#include <stdio.h>
#include <stdlib.h>
#include "l0_ism_governor.h"

int main(void) {
    printf("=== L0-ISM C99 RTOS Governor Verification ===\n\n");

    l0_state_vector_t test_suite[] = {
        { .p = 3, .t = 2, .q = 5, .strain = 1, .twist = L0_TWIST_Z_RIGHT }, /* Nominal: 3+2=5 (SAT) */
        { .p = 5, .t = 5, .q = 1, .strain = 2, .twist = L0_TWIST_S_LEFT  }, /* Violation: 5+5!=1 (UNSAT) */
        { .p = 9, .t = 0, .q = 0, .strain = 8, .twist = L0_TWIST_NEUTRAL }  /* Saturation: p=9 (UNSAT) */
    };

    size_t total_tests = sizeof(test_suite) / sizeof(test_suite);
    int failure_count = 0;

    for (size_t i = 0; i < total_tests; i++) {
        l0_action_code_t code = l0_ism_verify_state(&test_suite[i]);
        printf("Test Case %zu: Vector (%d, %d, %d) -> Action Code: %d [%s]\n",
               i + 1, test_suite[i].p, test_suite[i].t, test_suite[i].q, code,
               code == L0_ACTION_PERMIT ? "PERMIT (SAT)" : "HARD_STOP (UNSAT)");

        if (i == 0 && code != L0_ACTION_PERMIT) failure_count++;
        if (i > 0 && code == L0_ACTION_PERMIT) failure_count++;
    }

    if (failure_count == 0) {
        printf("\nAll C99 RTOS Governor tests passed successfully.\n");
        return EXIT_SUCCESS;
    } else {
        printf("\nC99 RTOS Governor verification FAILED (%d errors).\n", failure_count);
        return EXIT_FAILURE;
    }
}
Ścieżka: scripts/README.md
# L0-ISM Implementation Scripts & RTOS Engine

This directory contains reference implementations of the **Layer 0 Invariant State Matrix (L0-ISM)** architecture, as formalized in *Continuous Sobolev Local Smoothing vs. Discrete L0-ISM Hardware Interdiction: A Unified Data Physics Monograph*.

## Files Overview

* `test_z3_neuro_symbolic.py`: Python benchmark integrating PyTorch logits, Helical Venturi Operator $\Phi_v$, and CP-SAT/SMT Z3 sub-millisecond formal verification.
* `l0_ism_governor.h`: Zero-dependency, ISO C99 Header-Only RTOS governor for microcontrollers (ARM Cortex-M, ESP32, FPGA).
* `l0_ism_governor.c`: Verification driver testing $O(1)$ boundary saturation and zero-sum flux conservation.

## Quick Start & Testing

### C99 RTOS Governor Compilation
```bash
gcc -std=c99 -Wall -Wextra -Werror -O3 l0_ism_governor.c -o l0_governor_test
./l0_governor_test
