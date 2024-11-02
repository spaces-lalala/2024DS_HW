import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import math

# 執行 C++ 排序程式並生成 CSV
def run_sorting_program(executable_path, array_size, output_file, trials=10, timeout=7200):
    try:
        total_time = 0
        for _ in range(trials):
            result = subprocess.run(
                [executable_path, str(array_size)], capture_output=True, text=True, timeout=timeout
            )
            total_time += float(result.stdout.strip())
        
        # 計算平均時間並記錄到 CSV
        with open(output_file, 'a') as f:
            f.write(f"{array_size},{total_time / trials}\n")
        return True
    except subprocess.TimeoutExpired:
        print(f"Timeout for array size = {array_size}")
        return False

# 繪製圖二的折線圖
def plot_graph(filenames, labels, output_image):
    plt.figure(figsize=(10, 6))
    for filename, label in zip(filenames, labels):
        data = pd.read_csv(filename)
        plt.plot(data['size'], data['time'], label=label, marker='o')
        
    plt.xlabel("Array Size (2^k)")
    plt.ylabel("Average Time (s)")
    plt.title("Comparison of Randomized Quick Sort Variants Execution Time")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(output_image)
    plt.show()

def main():
    # 定義排序程式的可執行檔路徑
    sorting_programs = {
        "quick_lomuto": "./quick_lomuto",
        "quick_hoare": "./quick_hoare",
        "quick_3way": "./quick_3way"
    }
    
    output_files = {name: f"{name}_time.csv" for name in sorting_programs}
    
    # 初始化 CSV 檔案
    for file in output_files.values():
        with open(file, 'w') as f:
            f.write("size,time\n")
    
    # 生成不同大小的數組並執行排序
    for k in range(11, 21):  # 可調整範圍
        size = 2 ** k
        print(f"Running tests for array size = {size}")
        
        for name, executable in sorting_programs.items():
            run_sorting_program(executable, size, output_files[name])

    # 繪製結果
    plot_graph(list(output_files.values()), list(sorting_programs.keys()), "randomized_quick_sort_comparison.png")

if __name__ == "__main__":
    main()
