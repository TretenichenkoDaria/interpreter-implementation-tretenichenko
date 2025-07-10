import subprocess

tests = [
    ("var x = 7", "> 7"),
    ("x + 1", "> 8"),
    ("5 + 2 * 3", "> 11"),
    ("min(3 + 2, 4)", "> 4"),
    ("abs(-3 + min(1, 2)) * 2", "> 4"),
    ("max(2, 5)", "> 5"),
    ("pow(2, 3)", "> 8"),
]

input_text = ""
for expr, _ in tests:
    input_text += expr + "\n"

process = subprocess.Popen(
    ["./cmake-build-debug/interpreter_implementation_tretenichenko"],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    text=True
)

stdout, _ = process.communicate(input_text)
outputs = stdout.strip().splitlines()

for (expr, expected), actual in zip(tests, outputs):
    actual = actual.strip()
    if actual == expected:
        print(f"[PASS] {expr} → {actual}")
    else:
        print(f"[FAIL] {expr} → {actual}, expected {expected}")
