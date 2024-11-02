import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt

def run_cpp_program(executable_path, n_value, s_value, output_file, label):
    try:
        result = subprocess.run(
            [executable_path, str(n_value), str(s_value)],
            capture_output=True, text=True, timeout=7200
        )
        with open(output_file, 'a') as f:
            f.write(f"{n_value},{s_value},{label},{result.stdout.strip()}\n")
    except subprocess.TimeoutExpired:
        print(f"Execution for n={n_value}, s={s_value} timed out.")

def plot_graph(filename, output_image):
    data = pd.read_csv(filename)
    plt.figure(figsize=(12, 8))

    for label in sorted(data['label'].unique()):
        subset = data[data['label'] == label]
        plt.plot(subset['n'], subset['time'], label=label, marker='o')

    plt.xlabel("Number of Elements (n)")
    plt.ylabel("Time (seconds)")
    plt.title("Comparison of Quick_Merge_Sort_s and Merge_Quick_Sort_s with s = 1 to 5")
    plt.legend()
    plt.grid(True)
    plt.savefig(output_image)
    plt.show()

def main():
    n_values = [2**k for k in range(11, 16)]
    s_values = range(1, 6)
    output_file = "graph_three_data.csv"

    with open(output_file, 'w') as f:
        f.write("n,s,label,time\n")

    for s in s_values:
        for n in n_values:
            run_cpp_program("./quick_merge", n, s, output_file, f"Quick_Merge_Sort_s={s}")
            run_cpp_program("./merge_quick", n, s, output_file, f"Merge_Quick_Sort_s={s}")

    plot_graph(output_file, "graph_three.png")

if __name__ == "__main__":
    main()
