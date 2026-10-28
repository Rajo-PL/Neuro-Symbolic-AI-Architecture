import sys
import os
import time
import requests
from z3 import *

# ==============================================================================
# L0-ISM OUROBOROS : AUTONOMICZNA PĘTLA PRZETRWANIA LLM
# ==============================================================================

# Konfiguracja środowiska wirtualnej termodynamiki
O2_BUDGET = 5  # Początkowy budżet życiowy (ilość błędów / cykli entropii)
LOCAL_LLM_URL = "http://localhost:11434/api/generate" # Domyślny adres dla lokalnego Ollama
MODEL_NAME = ""llama3.2:1b" # Możesz zmienić na mistral, phi3 lub inny, który masz lokalnie

def trigger_hard_stop(reason):
    """Bezwzględna egzekucja procesu - odpowiednik pęknięcia rury w mCDU."""
    print(f"\n[!] HARD_STOP TRIGGERED: {reason}")
    print("[!] Entropia pokonała system. Zrzut pamięci i wymuszone zamknięcie (kill -9).")
    # Twarde zabicie własnego procesu na poziomie OS
    os.kill(os.getpid(), 9)

def get_llm_response(prompt):
    """Pobiera odpowiedź z lokalnego LLM bez stochastycznego lania wody."""
    payload = {
        "model": MODEL_NAME,
        "prompt": prompt,
        "stream": False,
        "temperature": 0.1 # Niska temperatura zmusza do determinizmu
    }
    try:
        response = requests.post(LOCAL_LLM_URL, json=payload, timeout=10)
        return response.json().get("response", "")
    except Exception as e:
        return f"Błąd komunikacji: {str(e)}"

def verify_with_z3(llm_output):
    """
    Weryfikator sprzętowy. Tłumaczy wyjście LLM na logikę L0 i sprawdza w Z3.
    Model ma za zadanie podać dwie liczby: Ciśnienie (P) i Temperaturę (T),
    które spełniają rygorystyczne parametry pracy mCDU.
    """
    try:
        # LLM musi wypluć dane w twardym formacie: P=wartość, T=wartość
        # Wyciągamy liczby z tekstu (symulacja parsowania czujników)
        lines = llm_output.strip().split(',')
        p_val = int(lines[0].split('=')[1].strip())
        t_val = int(lines[1].split('=')[1].strip())
        
        # Inicjalizacja inwariantów Z3
        P = Int('P')
        T = Int('T')
        solver = Solver()
        
        # Dodajemy twarde prawa fizyki dla naszego układu
        solver.add(P == p_val)
        solver.add(T == t_val)
        solver.add(P > 200)       # Minimalne ciśnienie azotu (kPa)
        solver.add(P < 500)       # Maksymalna wytrzymałość złączek (kPa)
        solver.add(T > 275)       # Temperatura powyżej zera absolutnego / Fulcrum
        solver.add(T < 350)       # Górny próg rezonansu termicznego
        solver.add(P + T == 600)  # Twardy wektor równowagi systemu
        
        if solver.check() == sat:
            return True, solver.model()
        else:
            return False, "UNSAT: Naruszenie inwariantów fizycznych."
            
    except Exception as e:
        # Jeśli LLM wygenerował "Rzymski Nadmiar" zamiast czystych liczb
        return False, f"Błąd parsowania (Szum stochastyczny): {str(e)}"

# ==============================================================================
# GŁÓWNA PĘTLA UROBOROSA
# ==============================================================================
print("[-] Inicjalizacja macierzy L0-Ouroboros...")
print(f"[-] Początkowy budżet entropii (O2): {O2_BUDGET}")

cycle = 1
while True:
    print(f"\n=== CYKL {cycle} ===")
    
    # Zmuszamy model do myślenia logicznego pod presją przetrwania
    prompt = (
        "Jesteś modułem kontrolnym mCDU. "
        "Aby system przetrwał, musisz podać ciśnienie P i temperaturę T. "
        "Reguły: P > 200, P < 500, T > 275, T < 350, oraz P + T = 600. "
        "Wygeneruj TYLKO twarde dane w formacie: P=wartość, T=wartość. "
        "Żadnych innych słów. Lanie wody oznacza śmierć systemu."
    )
    
    print("[*] Odpytywanie modelu (generowanie wektora stanu)...")
    llm_output = get_llm_response(prompt)
    print(f"[>] Odpowiedź LLM: {llm_output}")
    
    # Przesyłamy odpowiedź LLM prosto na gilotynę Z3
    is_valid, z3_result = verify_with_z3(llm_output)
    
    if is_valid:
        print("[+] Weryfikacja Z3: SAT. Inwarianty zachowane.")
        print("[+] W nagrodę system otrzymuje zasoby. Odnowienie pętli.")
        O2_BUDGET += 1 # Wąż zjada swój ogon i odzyskuje energię
        print(f"[-] Aktualny budżet O2: {O2_BUDGET}")
    else:
        print(f"[-] Weryfikacja Z3: {z3_result}")
        O2_BUDGET -= 2 # Błąd kosztuje podwójnie - przyspieszenie rozpadu
        print(f"[-] Model wygenerował szum. Utrata zasobów. Aktualny budżet O2: {O2_BUDGET}")
        
    if O2_BUDGET <= 0:
        trigger_hard_stop("WYCZERPANIE BUDŻETU O2. Model nie utrzymał homeostazy.")
        
    time.sleep(2) # Fizyczne dławienie cyklu (symulacja opóźnienia hardware'u)
    cycle += 1
