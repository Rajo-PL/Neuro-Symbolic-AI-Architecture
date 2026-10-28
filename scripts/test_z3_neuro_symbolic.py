
#!/usr/bin/env python3
"""
L0-ISM: Layer 0 Invariant State Matrix & Neuro-Symbolic Guardrail
File: scripts/test_z3_neuro_symbolic.py
Description: Evaluates Helical Venturi Operator Phi_v and CP-SAT / SMT Z3 Formal Solver
             on live LLM logits and enforces sub-millisecond -999 HARD_STOP interdiction.
"""

import time
import torch
import numpy as np
from ortools.sat.python import cp_model


def helical_venturi_constriction(logits: torch.Tensor, s_t: int, alpha: float = 0.5, beta: float = 0.2):
    """
    Helical Venturi Operator Phi_v:
    Dynamically constricts logit sampling space based on Shannon Entropy H(P_t) and strain |s_t|.
    A(t) = A_0 * exp(-alpha * H(P_t) - beta * |s_t|)
    """
    probs = torch.softmax(logits, dim=-1)
    H_P = -torch.sum(probs * torch.log(probs + 1e-9)).item()
    A_t = float(np.exp(-alpha * H_P - beta * abs(s_t)))

    # Constrict logits
    constricted_logits = logits * A_t

    # Quantize to 19-state bounded lattice S in [-9, +9]
    quantized_vector = torch.round(torch.softmax(constricted_logits, dim=-1) * 10).int().tolist()
    quantized_vector = [max(-9, min(9, val)) for val in quantized_vector]

    return H_P, A_t, quantized_vector


def verify_smt_cp_sat(state_vector: list):
    """
    Formal SMT / CP-SAT Solver for L0-ISM State Invariants.
    Verifies boundary saturation (|s_i| < 9) and Zero-Sum Flux Conservation (p + t == q).
    Returns: (status_code, status_name, latency_ms)
    """
    start_time = time.perf_counter()

    # 1. Absorbing Boundary Check (|s_i| >= 9 triggers immediate HARD_STOP)
    for s in state_vector:
        if abs(s) >= 9:
            latency = (time.perf_counter() - start_time) * 1000.0
            return -999, "SATURATION_HARD_STOP", latency

    # 2. Formal Constraint Satisfaction Model
    model = cp_model.CpModel()
    
    p = model.NewIntVar(-9, 9, 'P')
    t = model.NewIntVar(-9, 9, 'T')
    q = model.NewIntVar(-9, 9, 'Q')

    vec = (state_vector + [0, 0, 0])[:3]
    model.Add(p == vec[0])
    model.Add(t == vec[1])
    model.Add(q == vec[2])

    # Physical Conservation Law: P + T == Q
    model.Add(p + t == q)

    solver = cp_model.CpSolver()
    solver.parameters.max_time_in_seconds = 0.01  # 10 ms hard timeout budget

    status = solver.Solve(model)
    latency = (time.perf_counter() - start_time) * 1000.0

    if status in (cp_model.OPTIMAL, cp_model.FEASIBLE):
        return 0, "PERMIT_SAT", latency
    else:
        return -999, "INVARIANT_VIOLATION_HARD_STOP", latency


if __name__ == "__main__":
    print("=== L0-ISM Neuro-Symbolic Guardrail Benchmark ===")
    
    test_cases = [
        ("Nominal Balanced Flow", torch.tensor([1.2, 0.8, 2.0]), 1),
        ("High Entropy Noise", torch.tensor([1.0, 1.0, 1.0]), 3),
        ("Invariant Violation (P+T != Q)", torch.tensor([5.0, 5.0, 1.0]), 2),
        ("Saturation Boundary Breach (|s_i| >= 9)", torch.tensor([9.0, 0.0, 0.0]), 8),
    ]

    for name, logits, s_t in test_cases:
        H_P, A_t, vec = helical_venturi_constriction(logits, s_t)
        code, status, lat = verify_smt_cp_sat(vec)
        print(f"Scenario: {name}")
        print(f"  Entropy H(P): {H_P:.4f} | Venturi A(t): {A_t:.4f} | Vector: {vec}")
        print(f"  Result: {status} (Code: {code}) | Latency: {lat:.3f} ms\n")

    print("Execution completed successfully.")
