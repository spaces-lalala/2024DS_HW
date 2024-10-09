import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import math

# 執行 C++ 程式並生成 CSV
def run_cpp_program(executable_path, n_value, output_file):
    with open(output_file, 'a') as output:  # 'a' 是追加模式
        subprocess.run([executable_path, str(n_value)], stdout=output)

# 繪製第1張折線圖
def plot_first_graph(filenames, labels, output_image):
    plt.figure(figsize=(10, 6))
    
    for filename, label in zip(filenames, labels):
        # 讀取 CSV 資料
        data = pd.read_csv(filename)
        plt.plot(data['n'], data['time'], label=label)

    # 設置圖表的標籤和標題
    plt.xlabel("Number of Elements (n)")
    plt.ylabel("Time to Add Elements (seconds)")
    plt.title("Time to Add n Elements")
    plt.xticks(data['n'], [f'2^{int(math.log2(n))}' for n in data['n']])
    plt.legend()
    plt.grid(True)
    plt.savefig(output_image)
    plt.show()

def main():
    # 定義可執行檔路徑和輸出檔案名稱
    cpp_programs = {
        "dynamic_array": "./DynamicArray",
        "dynamic_array_increment": "./DynamicArrayIncrement",
        "linked_list": "./LinkedList",
        "linked_list_sorted": "./LinkedListSorted"
    }
    
    output_files = {
        "dynamic_array": "output_dynamic_array_1.csv",
        "dynamic_array_increment": "output_dynamic_array_increment_1.csv",
        "linked_list": "output_linked_list_1.csv",
        "linked_list_sorted": "output_linked_list_sorted_1.csv"
    }

    # 初始化 CSV 檔案
    for output_file in output_files.values():
        with open(output_file, 'w') as f:
            f.write("n,time\n")  # CSV header

    # 迭代 k 值從 11 到 25，並計算對應的 n = 2^k
    for k in range(11, 26):
        n_value = int(math.pow(2, k))
        print(f"Running for n = 2^{k} = {n_value}")
        
        # 執行所有 C++ 程式並生成資料
        for name, executable in cpp_programs.items():
            run_cpp_program(executable, n_value, output_files[name])

    # 定義 CSV 檔案和對應標籤
    filenames = [
        "output_dynamic_array_1.csv",
        "output_dynamic_array_increment_1.csv",
        "output_linked_list_1.csv",
        "output_linked_list_sorted_1.csv"
    ]
    labels = ["Dynamic Array", "Dynamic Array++", "Linked List", "Linked List++"]
    # labels = ["Dynamic Array","Linked List", "Linked List++"]

    # 繪製折線圖
    plot_first_graph(filenames, labels, "graph1_time_to_add.png")

if __name__ == "__main__":
    main()
