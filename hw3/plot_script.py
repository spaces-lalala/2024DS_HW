import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 執行 C++ 程式並生成 CSV
def run_cpp_program(executable_path, k_value, output_file):
    try:
        with open(output_file, 'a') as output:
            subprocess.run([executable_path, str(k_value)], stdout=output)
        return True
    except subprocess.TimeoutExpired:
        print(f"Execution for k = {k_value} timed out!")
        return False

# 繪製圖表# 繪製圖表函數
def plot_graph(filenames, labels, y_label, output_image, y_column):
    plt.figure(figsize=(12, 8))

    for filename, label in zip(filenames, labels):
        # 只選擇需要的欄位
        data = pd.read_csv(filename, usecols=["k", y_column])
        plt.plot(data['k'], data[y_column], label=label, marker='o')

    plt.xlabel("k (Number of swaps, log scale)")
    plt.ylabel(y_label)
    plt.xscale('log', base=2)
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(output_image)
    plt.show()

# 主程式
def main():
    # C++ 可執行檔
    cpp_programs = {
        "BST": "./BST",
        "AVL": "./AVL",
        "Treap": "./Treap",
        "SkipList": "./SkipList"
    }

    # 輸出文件
    output_files = {
        name: f"output_{name}.csv" for name in cpp_programs.keys()
    }

    # 初始化 CSV 檔案
    for file in output_files.values():
        with open(file, 'w') as f:
            f.write("k,value,height\n")

    # 測試 k 值範圍
    k_values = [2 ** i for i in range(1,2)]

    # 執行程式並收集資料
    for k in k_values:
        print(f"Running tests for k = {k}")
        for name, executable in cpp_programs.items():
            run_cpp_program(executable, k, output_files[name])

    # 繪製圖表
    plot_graph(
        list(output_files.values()),
        list(cpp_programs.keys()),
        "Insertion Time (s)",
        "graph_1.png",
        "value"  # 使用 'value' 欄位繪圖
    )

    plot_graph(
        list(output_files.values()),
        list(cpp_programs.keys()),
        "Height",
        "graph_2.png",
        "height"  # 使用 'height' 欄位繪圖
    )

if __name__ == "__main__":
    main()
