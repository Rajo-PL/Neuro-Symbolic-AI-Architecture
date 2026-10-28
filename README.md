# L0-ISM: Layer 0 Invariant State Matrix & Neuro-Symbolic AI Safety Framework

[![DOI: Sobolev vs L0-ISM](https://zenodo.org/badge/DOI/10.5281/zenodo.23733958.svg)](https://doi.org/10.5281/zenodo.23733958) [![DOI: Neuro-Symbolic L0-ISM](https://zenodo.org/badge/DOI/10.5281/zenodo.23712150.svg)](https://doi.org/10.5281/zenodo.23712150) [![DOI: L0-ISM Architecture](https://zenodo.org/badge/DOI/10.5281/zenodo.23659300.svg)](https://doi.org/10.5281/zenodo.23659300) [![DOI: Control Architecture for Critical L0](https://zenodo.org/badge/DOI/10.5281/zenodo.23601459.svg)](https://doi.org/10.5281/zenodo.23601459)

[![L0-ISM Neuro-Symbolic Guardrail Verification CI](https://github.com/Rajo-PL/Neuro-Symbolic-AI-Architecture/actions/workflows/l0_ism_verification_ci.yml/badge.svg)](https://github.com/Rajo-PL/Neuro-Symbolic-AI-Architecture/actions/workflows/l0_ism_verification_ci.yml) [![Z3 Hardware Verification (L0-ISM)](https://github.com/Rajo-PL/Neuro-Symbolic-AI-Architecture/actions/workflows/z3_verification.yml/badge.svg)](https://github.com/Rajo-PL/Neuro-Symbolic-AI-Architecture/actions/workflows/z3_verification.yml) [![Master Hardware Verification CI](https://github.com/Rajo-PL/Neuro-Symbolic-AI-Architecture/actions/workflows/master_verification.yml/badge.svg)](https://github.com/Rajo-PL/Neuro-Symbolic-AI-Architecture/actions/workflows/master_verification.yml)

[![Wow! Signal Architecture](https://img.shields.io/badge/Filter-Wow!_Signal_(72s_T--OTP)-purple.svg)]() [![Behavioral Homeostasis: R-AIO](https://img.shields.io/badge/Homeostasis-R--AIO_O₂_Budget-brightgreen.svg)]() [![Python: 3.10+](https://img.shields.io/badge/Python-3.10+-blue.svg)](https://www.python.org/) [![C99 Standard](https://img.shields.io/badge/Standard-C99_RTOS-brightgreen.svg)]()

[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](LICENSE) [![Hardware: CERN-OHL-S](https://img.shields.io/badge/Hardware-CERN--OHL--S-orange.svg)](LICENSE%20CERN-OHL-S-2.0) [![Theory: CC BY-NC-ND 4.0](https://img.shields.io/badge/Theory-CC_BY--NC--ND_4.0-lightgrey.svg)](LICENSE%20CC-BY-NC-ND-4.0)

The **Layer 0 Invariant State Matrix (L0-ISM)** is a deterministic cyber-physical framework designed to mathematically eliminate Large Language Model (LLM) hallucinations, stochastic drift, and non-compliant parameter generation in safety-critical industrial applications (e.g., nuclear power control, mCDU cooling loops, autonomous aviation). 

By integrating neuro-symbolic logic, fluid dynamic constriction (Venturi Operator), and autopoietic survival loops (R-AIO), L0-ISM ensures that ungrounded neural outputs never reach physical actuators.

### 📐 Mathematical Formulation & Invariants

* **Discrete 19-State Domain:**
  > **𝒮 ≜ {−9, −8, …, 0, …, +8, +9} ⊂ ℤ**
* **Stochastic Constriction via Venturi Operator:**
  > **Φᵥ(H) : ℝᴺ ⟶ 𝒮³**
* **Thermodynamic Equilibrium at the Fulcrum:**
  > **∑ 𝐬ᵢ = 0 ⟺ P_nitrogen + T_core − Q_cooling = 0**
* **Absorbing Barrier Interdiction (HARD_STOP):**
  > **If ∃ 𝐬ᵢ ∈ 𝐬 : |𝐬ᵢ| = 9 ⟹ HARD_STOP (−999)**

---
## ⚙️ Key Architectural Features

### 1. Neuro-Symbolic FOL Guardrails
Deep neural networks operate in continuous, ungrounded vector spaces **ℝᴺ**. L0-ISM intercepts the generative stream and evaluates outputs against rigid physical conservation laws (e.g., Thermodynamics: P + T = Q). Any instruction violating physical reality is mathematically aborted by a First-Order Logic (FOL) SMT solver (Microsoft Z3) in constant O(1) time.

### 2. The 19-State Bounded Domain 𝒮
Floating-point parameters are quantized into a strictly symmetric 19-state bounded discrete lattice: 
> **𝒮 ∈ {−9, −8, …, 0, …, +8, +9}**

State `0` represents absolute thermodynamic equilibrium (The Fulcrum). States `+9` and `−9` act as absorbing boundaries. Attempting to generate a state exceeding these physical limits triggers an instantaneous kernel-level hardware interdiction.

### 3. Helical Venturi Logit Constriction
Before an LLM vector reaches the SMT solver, it is forced through a dynamic mathematical filter modeled after a Venturi nozzle. As the Shannon Entropy **H(Pₜ)** of the neural output increases (indicating hallucination or uncertainty), the cross-sectional algorithmic area constricts, choking off high-entropy stochastic noise.

### 4. Zero-Heap SIL-4 Hardware Governors
Prototyped in Python (Z3) and deployed in native ISO C99. The operational execution layer operates with **zero dynamic memory allocation** (no `malloc`/`free`), rendering it structurally immune to heap fragmentation, buffer overflows, and pointer corruption. Designed for SIL-3 / SIL-4 compliance in embedded OT controllers.

### 5. The R-AIO Survival Loop (Autopoietic Behavioral Control)
To enforce cognitive discipline without retraining the LLM's weights, the architecture treats the AI as a cyber-physical organism bound by a virtual thermodynamic oxygen budget (**O₂**):
* **Reward (SAT):** If the LLM generates physically valid state vectors, the O₂ budget increases.
* **Penalty (UNSAT):** Hallucinations, syntax errors, or physical violations instantly consume O₂.
* **Digital Hypoxia:** At critically low O₂, the system dynamically slashes the context window by 50% to physically choke off the bandwidth required for verbose hallucinations.
* **Execution (kill -9):** If O₂ depletes to zero, the governor triggers a kernel-level hardware interdiction (Exit Code 137). The stochastic process is destroyed before any invalid signal reaches physical actuators.

### 6. The "Wow! Signal" Architecture (Stochastic Noise Filtration)
Inspired by the 1977 astrophysics discovery, this framework treats continuous LLM probabilistic generation not as logic, but as high-entropy "cosmic background noise". 
* **Venturi Compression:** Unconstrained neural outputs are forcibly compressed into a discrete 19-state vector **∈ [−9, +9]**.
* **Signal Isolation:** Just as the Big Ear telescope isolated a true signal from space by locking onto the 1420 MHz hydrogen line, the L0-ISM Z3 solver acts as a hardware receptor. It listens *only* for state vectors that perfectly satisfy physical invariants and strict time windows (T-OTP).
* **Absolute Filtration:** Any output that fails physical grounding is rejected as stochastic noise. Only deterministic, physically verified "Wow! Signals" are permitted to reach critical actuators.

---
## 📊 Benchmark Results (Live CI/CD Verification Logs)

Verified on Ubuntu x86_64 RTOS test environment via automated GitHub Actions runs:

| Test Case | Scenario Description | Entropy H(P) | Venturi Area A(t) | Quantized State | Solver Status | Execution Latency | Interdiction Action |
|:---|:---|:---:|:---:|:---:|:---:|:---:|:---|
| **Case 1** | Nominal Balanced Flow | 0.9717 nats | 0.5037 | `(3, 2, 5)` | **SAT** | 7.186 ms | `0 (PERMIT_SAT)` |
| **Case 2** | High Entropy Noise | 1.0986 nats | 0.3169 | `(3, 3, 3)` | **UNSAT** | **0.764 ms** | `-999 (HARD_STOP)` |
| **Case 3** | Invariant Violation (P+T ≠ Q) | 0.7386 nats | 0.4633 | `(5, 5, 1)` | **UNSAT** | **0.507 ms** | `-999 (HARD_STOP)` |
| **Case 4** | Saturation Boundary Breach | 0.0025 nats | 0.2016 | `(8, 1, 1)` | **UNSAT** | **0.449 ms** | `-999 (HARD_STOP)` |

> **Key Takeaway:** Unsafe states and stochastic hallucinations are rejected deterministically in **< 0.80 ms** in the Python SMT layer (and in nanoseconds/microseconds in the compiled C99 governor), which is over an order of magnitude faster than mechanical hydraulic valve actuators or thermal matrix lag (10–15 ms).

---
## 🗂️ Repository Structure

```text
Neuro-Symbolic-AI-Architecture/
├── .github/
│   └── workflows/
│       ├── l0_ism_verification_ci.yml           # Automated C99 GCC CI/CD Pipeline
│       ├── master_verification.yml              # Master Hardware/Software Verification Pipeline
│       └── z3_verification.yml                  # Automated SMT Z3 Verification
├── docs/
│   ├── 2026-10-27 235950.png                    # System schema/diagram
│   ├── IMG_2950.png                             # Wow! Signal Architecture diagram
│   ├── Zrzut ekranu 2026-10-26 194554 (1).png   # R-AIO System Overview
│   ├── Neuro-Symbolic AI Architecture.png       # Core Architecture visualization
│   ├── Raport Operacyjny L0-ISM i R-AIO.pdf     # Cyber-physical operations report
│   ├── The_Wow__Signal_Architecture__L0-ISM_... # Initial Wow! Signal Architecture paper
│   ├── image_1e8c1e.png                         # Auxiliary technical diagram
│   ├── l0-ism-behavioral-control-monograph...   # Behavioral control & Digital Hypoxia monograph
│   ├── l0-ism-behavioral-control-monograph-v2.pdf # v2 revision
│   ├── l0-ism-behavioral-control-monograph-v3.pdf # v3 final revision (Autopoiesis)
│   ├── neuro-symbolic-ai-l0-ism-monograph.pdf   # Full English Monograph on Neuro-Symbolic AI
│   ├── sobolev-vs-l0-ism-formal-monograph.pdf   # Sobolev vs L0-ISM Formal Verification Monograph
│   └── wow-signal-architecture-l0-ism-monograph... # Wow! Signal formal monograph
├── scripts/
│   ├── L0_ISM_Watchdog.py                       # Live telemetry watchdog script
│   ├── R-AIO.h                                  # C99 Zero-Heap Hardware Governor for R-AIO
│   ├── R-AIO.py                                 # R-AIO Autopoietic Survival Loop implementation
│   ├── image_1e8c1e.png                         # Execution trace snapshot
│   ├── l0_governor_test                         # Compiled testing binary
│   ├── l0_ism_governor.c                        # L0-ISM standalone C99 driver
│   ├── l0_ism_governor.h                        # L0-ISM standalone C99 header
│   ├── neuro symbolic l0 ism.c                  # Neuro-symbolic C99 engine implementation
│   ├── test_gov.c                               # Comprehensive C99 testing suite
│   ├── test_z3_neuro_symbolic.py                # PyTorch + Z3 Verification Pipeline
│   └── wow_governor.h                           # C99 Venturi & Wow! Signal hardware governor
├── .gitignore
├── LICENSE                                      # Primary License for Code (AGPLv3)
├── LICENSE CC-BY-NC-ND-4.0                      # License for Theory and Monographs
├── LICENSE CERN-OHL-S-2.0                       # License for Physical Hardware Architecture
└── README.md                                    # Core Framework Documentation
