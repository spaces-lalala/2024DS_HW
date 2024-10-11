import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import math
import numpy as np

# 執行 C++ 程式並生成 CSV
def run_cpp_program(executable_path, output_file, mode='third'):
    with open(output_file, 'a') as output:  # 'a' 是追加模式
        subprocess.run([executable_path, "dummy_arg", mode], stdout=output)


# 繪製第1張折線圖
def plot_third_graph(filenames, labels, output_image):
    plt.figure(figsize=(10, 6))
    
    for filename, label in zip(filenames, labels):
        # 讀取 CSV 資料
        data = pd.read_csv(filename)

        plt.plot(data['k'], data['time'], label=label, marker='o')

    # 設置圖表的標籤和標題
    plt.xlabel("k (Each representing the addition of 2^13 elements)")
    plt.ylabel("Time to Add 2^13 Elements (seconds)")
    plt.title("Time to Add Elements in blocks of 2^13 plot")

    plt.legend()
    plt.grid(True)
    
    # 儲存並顯示圖表
    plt.tight_layout()
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
        "dynamic_array": "output_3_DA.csv",
        "dynamic_array_increment": "output_3_DA++.csv",
        "linked_list": "output_3_LL.csv",
        "linked_list_sorted": "output_3_LL++.csv"
    }

    # 初始化 CSV 檔案
    for output_file in output_files.values():
        with open(output_file, 'w') as f:
            f.write("k,time\n")  # CSV header

    # 迭代 k 值從 11 到 25，並計算對應的 n = 2^k
    
        print(f"Running for n = 2^20")
        
        # 執行所有 C++ 程式並生成資料
        for name, executable in cpp_programs.items():            
            run_cpp_program(executable, output_files[name])
                 

    # 定義 CSV 檔案和對應標籤
    filenames = [
        "output_3_DA.csv",
        "output_3_DA++.csv",
        "output_3_LL.csv",
        "output_3_LL++.csv"
    ]
    labels = ["Dynamic Array", "Dynamic Array++", "Linked List", "Linked List++"]

    # 繪製折線圖
    plot_third_graph(filenames, labels, "graph3_time_to_add_per_2^13.png")

if __name__ == "__main__":
    main()
