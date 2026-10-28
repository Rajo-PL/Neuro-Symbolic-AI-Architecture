/**
 * @file l0_ism_governor.h
 * @brief Layer 0 Invariant State Matrix (L0-ISM) C99 RTOS Real-Time Governor
 * 
 * ISO/IEC 9899:1999 compliant hardware interdiction engine for Cyber-Physical AI Systems.
 * Guarantees O(1) constant-time boundary checking and zero-sum flux validation.
 */

#ifndef L0_ISM_GOVERNOR_H
#define L0_ISM_GOVERNOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* === L0-ISM Hardware Constants === */
#define L0_ISM_LATTICE_MIN      (-9)
#define L0_ISM_LATTICE_MAX      (+9)
#define L0_ISM_FULCRUM          (0)
#define L0_ISM_HARD_STOP        (-999)
#define L0_ISM_PERMIT           (0)

/* === Interdiction Action Codes === */
typedef enum {
    L0_ACTION_PERMIT                  =  0,
    L0_ACTION_HARD_STOP_SATURATION    = -999,
    L0_ACTION_HARD_STOP_INVARIANT     = -998,
    L0_ACTION_HARD_STOP_PARITY_FLAP   = -997
} l0_action_code_t;

/* === Chiral Parity Twist === */
typedef enum {
    L0_TWIST_S_LEFT  = -1,  /* Drainage / Outflow */
    L0_TWIST_NEUTRAL =  0,  /* Fulcrum Equilibrium */
    L0_TWIST_Z_RIGHT = +1   /* Supply / Inflow */
} l0_twist_t;

/* === Quantized State Vector Struct === */
typedef struct {
    int8_t p;             /* State variable P */
    int8_t t;             /* State variable T */
    int8_t q;             /* State variable Q */
    int8_t strain;        /* System strain s_t */
    l0_twist_t twist;     /* Chiral parity twist */
} l0_state_vector_t;

/**
 * @brief Fast O(1) Deterministic Verification of L0-ISM State Vector.
 * @param state Pointer to quantized state vector struct.
 * @return L0_ACTION_PERMIT (0) on SAT, or negative HARD_STOP code on UNSAT.
 */
static inline l0_action_code_t l0_ism_verify_state(const l0_state_vector_t* state) {
    if (!state) return L0_ACTION_HARD_STOP_INVARIANT;

    /* 1. Absorbing Boundary Check (|s_i| >= 9) */
    if (state->p <= L0_ISM_LATTICE_MIN || state->p >= L0_ISM_LATTICE_MAX ||
        state->t <= L0_ISM_LATTICE_MIN || state->t >= L0_ISM_LATTICE_MAX ||
        state->q <= L0_ISM_LATTICE_MIN || state->q >= L0_ISM_LATTICE_MAX) {
        return L0_ACTION_HARD_STOP_SATURATION;
    }

    /* 2. Zero-Sum Flux Conservation Law: P + T == Q */
    if ((int16_t)state->p + (int16_t)state->t != (int16_t)state->q) {
        return L0_ACTION_HARD_STOP_INVARIANT;
    }

    return L0_ACTION_PERMIT;
}

#ifdef __cplusplus
}
#endif

#endif /* L0_ISM_GOVERNOR_H */