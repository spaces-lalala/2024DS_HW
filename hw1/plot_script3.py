import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import math
from sklearn.linear_model  import LinearRegression
import numpy as np

# 執行 C++ 程式並生成 CSV
def run_cpp_program(executable_path, n_value, output_file, mode='third', timeout=60):
    try:
        with open(output_file, 'a') as output:  # 'a' 是追加模式
            subprocess.run([executable_path, str(n_value), mode], stdout=output, timeout=timeout)
        return True
    except subprocess.TimeoutExpired:
        print(f"Test for n = {n_value} timed out!")
        return False

# 繪製第3張折線圖
def plot_third_graph(filenames, labels, output_image, missing_points):
    plt.figure(figsize=(10, 6))
    
    for filename, label in zip(filenames, labels):
        # 讀取 CSV 資料
        data = pd.read_csv(filename)
        plt.plot(data['k'], data['time'], label=label, marker='o')

        # 標示預測點
        if label in missing_points:
            missing_data = missing_points[label]
            plt.scatter(missing_data['n'], missing_data['predicted_time'], label=f"{label} (predicted)", color='red', marker='x')

    # 設置圖表的標籤和標題
    plt.xlabel("k (Each representing the addition of 2^13 elements)")
    plt.ylabel("Time to Add 2^13 Elements (seconds)")
    plt.title("Time to Add Elements in blocks of 2^13")

    plt.legend()
    plt.grid(True)
    
    # 儲存並顯示圖表
    plt.tight_layout()
    plt.savefig(output_image)
    plt.show()


# 使用線性回歸來填補缺失的資料點
def predict_missing_points(csv_file):
    # 讀取資料
    data = pd.read_csv(csv_file)
    
    # 確保有足夠的資料來做預測
    if len(data) < 2:
        return None
    
    # 取出 n 和 time 欄位
    X = np.log2(data['n']).values.reshape(-1, 1)  # 將 n 取對數
    y = data['time'].values
    
    # 使用線性回歸來預測
    model = LinearRegression()
    model.fit(X, y)

    return model

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

    # 初始化 CSV 檔案，並新增 k 和 time 欄位
    for output_file in output_files.values():
        with open(output_file, 'w') as f:
            f.write("k,time\n")  # CSV header

    # 定義每次實驗的 n 值和總階段數
    total_n_value = int(math.pow(2, 20))
    block_size = int(math.pow(2, 13))  # 每次新增的數量
    
    # 執行 128 個階段的實驗
    for k in range(1, 129):
        print(f"Running for k = {k} (adding 2^13 elements)")
        for name, executable in cpp_programs.items():
            run_cpp_program(executable, block_size, output_files[name])

    # 定義 CSV 檔案和對應標籤
    filenames = [
        "output_3_DA.csv",
        "output_3_DA++.csv",
        "output_3_LL.csv",
        "output_3_LL++.csv"
    ]
    labels = ["Dynamic Array", "Dynamic Array++", "Linked List", "Linked List++"]

    # 繪製折線圖
    plot_third_graph(filenames, labels, "graph3_time_to_add_2_13.png")

if __name__ == "__main__":
    main()
