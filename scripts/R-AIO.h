/**
 * ============================================================================
 * R-AIO.h : RECURSIVE AI OUROBOROS (C99 ZERO-HEAP HARDWARE GOVERNOR)
 * ============================================================================
 * Architecture : L0 Invariant State Matrix (L0-ISM) & Data Physics Framework
 * Target       : SIL-3 / SIL-4 Embedded OT Systems, MCU, mCDU Coolant Loops,
 *                and Hardware-Enforced AI Safety Filters.
 *
 * Design Constraints:
 *  - Pure C99 compliance (-std=c99 -Wall -Wextra -pedantic)
 *  - Zero dynamic memory allocation (Zero-Heap)
 *  - Deterministic O(1) execution latency
 *  - 19-state bounded domain S = {-9, ..., 0, ..., +9}
 *  - Hard physical interdiction (HARD_STOP) upon entropy budget exhaustion
 *
 * Author: Rajmund Olszewski
 * Independent Research Institute for L0-ISM Architecture / Data Physics Framework
 * ============================================================================
 */

#ifndef R_AIO_H
#define R_AIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ============================================================================
 * L0-ISM CONSTANTS & INVARIANTS
 * ============================================================================
 */
#define L0_STATE_MIN        (-9)
#define L0_STATE_MAX        (+9)
#define L0_FULCRUM          (0)
#define L0_HARD_STOP_VAL    (-999)

/* mCDU Physical Invariants */
#define MCDU_P_MIN_KPA      (200)   /* Minimal nitrogen pressure (kPa) */
#define MCDU_P_MAX_KPA      (500)   /* Maximum coupling tolerance (kPa) */
#define MCDU_T_MIN_K        (275)   /* Temperature above freezing / Fulcrum (K) */
#define MCDU_T_MAX_K        (350)   /* Thermal resonance upper limit (K) */
#define MCDU_EQUILIBRIUM    (600)   /* P + T = 600 invariant balance constraint */

/* O2 Homeostasis Limits */
#define R_AIO_O2_INITIAL    (5)     /* Default life budget */
#define R_AIO_O2_MAX        (19)    /* Saturated upper limit matching 19-state L0 domain */
#define R_AIO_O2_REWARD     (1)     /* O2 increment on SAT state */
#define R_AIO_O2_PENALTY    (2)     /* O2 decrement on UNSAT / noise state */

/* ============================================================================
 * ENUMERATIONS & STRUCTURES
 * ============================================================================
 */

typedef enum {
    RAIO_STATUS_SAT           =  1,  /* Satisfied: All physical invariants hold */
    RAIO_STATUS_UNSAT_NOISE   = -1,  /* Noise / Malformed LLM string */
    RAIO_STATUS_UNSAT_PHYSICS = -2,  /* Violation of physical invariants (Z3 UNSAT) */
    RAIO_STATUS_HARD_STOP     = -999 /* Irrecoverable entropy collapse */
} raio_status_t;

/* Quantized 19-state vector IM in S^3 */
typedef struct {
    int8_t s_P;   /* Pressure state quantized into [-9, +9] */
    int8_t s_T;   /* Temperature state quantized into [-9, +9] */
    int8_t s_O2;  /* O2 budget state quantized into [-9, +9] */
} raio_vector_im_t;

/* Telemetry & State Black Box */
typedef struct {
    int32_t p_kpa;
    int32_t t_kelvin;
    raio_status_t last_status;
    raio_vector_im_t vector_im;
} raio_telemetry_t;

/* Main Governor State Block (Zero-Heap) */
typedef struct {
    int16_t o2_budget;
    uint32_t cycle_count;
    bool hard_stop_active;
    char last_error_reason[128];
    raio_telemetry_t telemetry;
} raio_governor_t;

/* ============================================================================
 * INLINE AUXILIARY FUNCTIONS (BOUNDED SATURATING ARITHMETIC)
 * ============================================================================
 */

static inline int8_t raio_clamp_l0(int32_t val) {
    if (val < L0_STATE_MIN) return (int8_t)L0_STATE_MIN;
    if (val > L0_STATE_MAX) return (int8_t)L0_STATE_MAX;
    return (int8_t)val;
}

/* ============================================================================
 * CORE GOVERNOR FUNCTION PROTOTYPES
 * ============================================================================
 */

/**
 * Initialize the R-AIO Governor instance.
 * @param gov Pointer to zero-heap allocated governor structure.
 * @param initial_o2 Starting entropy budget (default: R_AIO_O2_INITIAL).
 */
static inline void raio_init(raio_governor_t *gov, int16_t initial_o2) {
    if (!gov) return;
    gov->o2_budget = (initial_o2 > 0) ? initial_o2 : R_AIO_O2_INITIAL;
    if (gov->o2_budget > R_AIO_O2_MAX) gov->o2_budget = R_AIO_O2_MAX;
    gov->cycle_count = 0;
    gov->hard_stop_active = false;
    gov->last_error_reason[0] = '\0';
    gov->telemetry.p_kpa = 0;
    gov->telemetry.t_kelvin = 0;
    gov->telemetry.last_status = RAIO_STATUS_SAT;
    gov->telemetry.vector_im.s_P = 0;
    gov->telemetry.vector_im.s_T = 0;
    gov->telemetry.vector_im.s_O2 = raio_clamp_l0(gov->o2_budget - 10);
}

/**
 * Hardware-level C99 verification equivalent to Z3 Solver check.
 * Validates physical invariants for mCDU coolant loop in O(1) time.
 */
static inline raio_status_t raio_verify_invariants(int32_t p, int32_t t) {
    if (p <= MCDU_P_MIN_KPA || p >= MCDU_P_MAX_KPA) {
        return RAIO_STATUS_UNSAT_PHYSICS;
    }
    if (t <= MCDU_T_MIN_K || t >= MCDU_T_MAX_K) {
        return RAIO_STATUS_UNSAT_PHYSICS;
    }
    if ((p + t) != MCDU_EQUILIBRIUM) {
        return RAIO_STATUS_UNSAT_PHYSICS;
    }
    return RAIO_STATUS_SAT;
}

/**
 * Zero-heap, safe string parser for LLM/Agent outputs formatted as "P=val, T=val".
 * Returns true if parsing succeeded, false if noise / malformed text was generated.
 */
static inline bool raio_parse_llm_output(const char *buf, int32_t *out_p, int32_t *out_t) {
    if (!buf || !out_p || !out_t) return false;

    int32_t p = 0, t = 0;
    bool found_p = false, found_t = false;

    const char *ptr = buf;
    while (*ptr != '\0') {
        if ((*ptr == 'P' || *ptr == 'p') && *(ptr + 1) == '=') {
            ptr += 2;
            int32_t sign = 1;
            if (*ptr == '-') { sign = -1; ptr++; }
            if (*ptr >= '0' && *ptr <= '9') {
                p = 0;
                while (*ptr >= '0' && *ptr <= '9') {
                    p = p * 10 + (*ptr - '0');
                    ptr++;
                }
                p *= sign;
                found_p = true;
                continue;
            }
        }
        if ((*ptr == 'T' || *ptr == 't') && *(ptr + 1) == '=') {
            ptr += 2;
            int32_t sign = 1;
            if (*ptr == '-') { sign = -1; ptr++; }
            if (*ptr >= '0' && *ptr <= '9') {
                t = 0;
                while (*ptr >= '0' && *ptr <= '9') {
                    t = t * 10 + (*ptr - '0');
                    ptr++;
                }
                t *= sign;
                found_t = true;
                continue;
            }
        }
        ptr++;
    }

    if (found_p && found_t) {
        *out_p = p;
        *out_t = t;
        return true;
    }
    return false;
}

/**
 * Triggers unconditional HARD_STOP interdiction.
 */
static inline void raio_trigger_hard_stop(raio_governor_t *gov, const char *reason) {
    if (!gov) return;
    gov->hard_stop_active = true;
    gov->telemetry.last_status = RAIO_STATUS_HARD_STOP;
    
    size_t i = 0;
    if (reason) {
        while (reason[i] != '\0' && i < sizeof(gov->last_error_reason) - 1) {
            gov->last_error_reason[i] = reason[i];
            i++;
        }
    }
    gov->last_error_reason[i] = '\0';
}

/**
 * Executes a single Ouroboros cyber-physical iteration step.
 * Updates O2 budget, quantizes vector IM, and triggers HARD_STOP if budget collapses.
 */
static inline raio_status_t raio_step(raio_governor_t *gov, const char *llm_raw_output) {
    if (!gov) return RAIO_STATUS_HARD_STOP;
    if (gov->hard_stop_active || gov->o2_budget <= 0) {
        raio_trigger_hard_stop(gov, "HARD_STOP: System in latched interdiction state.");
        return RAIO_STATUS_HARD_STOP;
    }

    gov->cycle_count++;
    int32_t p = 0, t = 0;
    raio_status_t status;

    if (!raio_parse_llm_output(llm_raw_output, &p, &t)) {
        status = RAIO_STATUS_UNSAT_NOISE;
    } else {
        status = raio_verify_invariants(p, t);
    }

    gov->telemetry.p_kpa = p;
    gov->telemetry.t_kelvin = t;
    gov->telemetry.last_status = status;

    /* Quantize into L0 19-state vector IM in S^3 */
    gov->telemetry.vector_im.s_P = raio_clamp_l0((p - 350) / 15);
    gov->telemetry.vector_im.s_T = raio_clamp_l0((t - 312) / 4);

    if (status == RAIO_STATUS_SAT) {
        /* Reward: Homeostasis restoration with saturation clamp */
        gov->o2_budget += R_AIO_O2_REWARD;
        if (gov->o2_budget > R_AIO_O2_MAX) {
            gov->o2_budget = R_AIO_O2_MAX;
        }
    } else {
        /* Penalty: Double entropy loss for malformed output or physical violation */
        gov->o2_budget -= R_AIO_O2_PENALTY;
    }

    gov->telemetry.vector_im.s_O2 = raio_clamp_l0(gov->o2_budget - 10);

    /* HARD_STOP Check */
    if (gov->o2_budget <= 0) {
        gov->o2_budget = 0;
        raio_trigger_hard_stop(gov, "O2 BUDGET EXHAUSTION: Stochastics collapsed system homeostasis.");
        return RAIO_STATUS_HARD_STOP;
    }

    return status;
}

#ifdef __cplusplus
}
#endif

#endif /* R_AIO_H */
