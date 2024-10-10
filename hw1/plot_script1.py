import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import math
from sklearn.linear_model  import LinearRegression
import numpy as np

# 執行 C++ 程式並生成 CSV
def run_cpp_program(executable_path, n_value, output_file, mode='first', timeout=60000):
    try:
        with open(output_file, 'a') as output:  # 'a' 是追加模式
            subprocess.run([executable_path, str(n_value), mode], stdout=output, timeout=timeout)
        return True
    except subprocess.TimeoutExpired:
        print(f"Test for n = {n_value} timed out!")
        return False

# 繪製第1張折線圖
def plot_first_graph(filenames, labels, output_image, missing_points):
    plt.figure(figsize=(10, 6))
    
    for filename, label in zip(filenames, labels):
        # 讀取 CSV 資料
        data = pd.read_csv(filename)

        # 使用 range(len(data['n'])) 作為 X 軸，保證間隔相同
        plt.plot(range(len(data['n'])), data['time'], label=label, marker='o')
        
        # 標示預測點
        if label in missing_points:
            missing_data = missing_points[label]
            plt.scatter(missing_data['n'], missing_data['predicted_time'], label=f"{label} (predicted)", color='red', marker='x')

    # 設置圖表的標籤和標題
    plt.xlabel("Number of Elements (n)")
    plt.ylabel("Time to Add Elements (seconds)")
    plt.title("Time to Add n Elements")


    # 將 X 軸標籤設置為等距的 2^k 標籤
    xticks_labels = [f'2^{int(math.log2(n))}' for n in data['n']]
    plt.xticks(range(len(xticks_labels)), xticks_labels)  # X 軸標籤應該是等距的

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
    X = data['n'].values.reshape(-1, 1)  # 將 n 取對數
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
        "dynamic_array": "output_1_DA.csv",
        "dynamic_array_increment": "output_1_DA++.csv",
        "linked_list": "output_1_LL.csv",
        "linked_list_sorted": "output_1_LL++.csv"
    }

    # 初始化 CSV 檔案
    for output_file in output_files.values():
        with open(output_file, 'w') as f:
            f.write("n,time\n")  # CSV header

    # 紀錄哪些資料點超時並需要預測
    missing_points = {
        "dynamic_array": {"n": [], "predicted_time": []},
        "dynamic_array_increment": {"n": [], "predicted_time": []},
        "linked_list": {"n": [], "predicted_time": []},
        "linked_list_sorted": {"n": [], "predicted_time": []}
    }

    # 記錄哪些資料結構已經跳過剩餘計算
    skipped_structures = {
        "dynamic_array": False,
        "dynamic_array_increment": False,
        "linked_list": False,
        "linked_list_sorted": False
    }


    # 迭代 k 值從 11 到 25，並計算對應的 n = 2^k
    for k in range(11, 26):
        n_value = int(math.pow(2, k))
        print(f"Running for n = 2^{k} = {n_value}")
        
        # 執行所有 C++ 程式並生成資料
        for name, executable in cpp_programs.items():
            # 如果該資料結構之前已經被跳過，則不再執行，直接將後續點加入 missing_points
            if skipped_structures[name]:
                print(f"Skipping {name} for n = 2^{k}")
                missing_points[name]["n"].append(n_value)
                continue  # 繼續處理下一個資料結構
            
            # 執行程式並檢查是否成功
            success = run_cpp_program(executable, n_value, output_files[name], timeout=60000)
            
            # 若超過時間限制，則記錄需要預測的點並標記為跳過後續
            if not success:
                print(f"Skipping remaining points for {name} after n = 2^{k}")
                missing_points[name]["n"].append(n_value)
                skipped_structures[name] = True  # 標記此資料結構為跳過

    # 預測缺失資料點並將預測值加入 CSV
    for name, output_file in output_files.items():
        # 使用現有資料進行回歸模型預測
        model = predict_missing_points(output_file)
        
        if model and missing_points[name]["n"]:
            for n_value in missing_points[name]["n"]:
                predicted_time = model.predict(np.array([[n_value]]))[0]
                with open(output_file, 'a') as f:
                    f.write(f"{n_value},{predicted_time}\n")
                
                # 保存預測資料點
                missing_points[name]["predicted_time"].append(predicted_time)
        else:
            missing_points[name] = {"n": [], "predicted_time": []}            

    # 定義 CSV 檔案和對應標籤
    filenames = [
        "output_1_DA.csv",
        "output_1_DA++.csv",
        "output_1_LL.csv",
        "output_1_LL++.csv"
    ]
    labels = ["Dynamic Array", "Dynamic Array++", "Linked List", "Linked List++"]
    # labels = ["Dynamic Array","Linked List", "Linked List++"]

    # 繪製折線圖
    plot_first_graph(filenames, labels, "graph1_time_to_add.png", missing_points)

if __name__ == "__main__":
    main()
