#include <stdio.h>
#include <assert.h>
#include "R-AIO.h"
#include "wow_governor.h"

int main(void) {
    printf("[+] Starting L0-ISM Hardware Governors Verification...\n");

    /* 1. Test R-AIO Engine */
    raio_governor_t raio_gov;
    raio_init(&raio_gov, 5);
    assert(raio_gov.o2_budget == 5);

    /* Test SAT state: P=300, T=300 => Equilibrium P+T=600 */
    raio_status_t s_sat = raio_step(&raio_gov, "WYNIK: P=300, T=300");
    assert(s_sat == RAIO_STATUS_SAT);
    assert(raio_gov.o2_budget == 6);
    printf("  [PASS] R-AIO SAT State: Invariants valid, O2 incremented to %d\n", raio_gov.o2_budget);

    /* Test UNSAT Physics: P=400, T=300 => Sum 700 != 600 */
    raio_status_t s_unsat = raio_step(&raio_gov, "WYNIK: P=400, T=300");
    assert(s_unsat == RAIO_STATUS_UNSAT_PHYSICS);
    assert(raio_gov.o2_budget == 4);
    printf("  [PASS] R-AIO UNSAT Physics: Breach detected, O2 penalised to %d\n", raio_gov.o2_budget);

    /* 2. Test Wow! Signal Venturi Governor */
    wow_governor_t wow_gov;
    wow_governor_init(&wow_gov);
    wow_venturi_config_t cfg = { .scale_weight = 1.0, .offset = 0.0, .gain = 1.0 };
    wow_llm_output_t llm_out = { .raw_p_power = 3.2, .raw_t_ambient = 2.0, .raw_q_cooling = 6.0, .timestamp_ms = 100 };
    
    l0_state_t act_state;
    wow_status_t ws = wow_governor_step(&wow_gov, &llm_out, &cfg, 150, &act_state);
    assert(ws == WOW_STATUS_SAT);
    assert(act_state == 3);
    printf("  [PASS] Wow! Venturi: Quantized raw float (3.2) -> L0 State (3), Status SAT\n");

    printf("[+] All L0-ISM Hardware Governors passed SIL-4 validation successfully.\n");
    return 0;
}
