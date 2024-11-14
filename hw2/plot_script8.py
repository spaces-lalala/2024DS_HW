import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import math
from sklearn.linear_model import LinearRegression
import numpy as np
 
# 執行 C++ 排序程式並生成 CSV
def run_cpp_program(executable_path, n_value, mode, k_value, output_file, timeout=3000):
    try:
        with open(output_file, 'a') as output:
            subprocess.run([executable_path, str(n_value), mode, str(k_value)], stdout=output, timeout=timeout)
        return True
    except subprocess.TimeoutExpired:
        print(f"Test for k = {k_value} timed out!")
        return False

# 繪製圖一的折線圖
def plot_graph(filenames, labels, output_image, missing_points):
    plt.figure(figsize=(10, 6))
    
    # 建立顏色映射，確保每個排序方法的預測點和線條顏色一致
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c']  # 默認顏色循環
    
    for (filename, label), color in zip(zip(filenames, labels), colors):
        # 讀取 CSV 資料
        data = pd.read_csv(filename)
        x_ticks = range(len(data['k_value']))
        
        # 先畫折線和點
        plt.plot(x_ticks, data['time'], label=label, marker='o', color=color)
        
        # 再畫預測點（x標記），這樣確保x會在o上面
        if label in missing_points:
            missing_data = missing_points[label]
            missing_x_ticks = [data[data['k_value'] == k_value].index[0] for k_value in missing_data['k_value']]
            plt.scatter(missing_x_ticks, missing_data['predicted_time'], 
                       color=color, marker='x', s=100, zorder=5)  # 增加 zorder 確保在其他元素上面

    # 設置圖表的標籤和標題
    plt.xlabel("k_value (k)")
    plt.ylabel("Time to sort (seconds)")
    plt.title("Graph8")

    # 設定 X 軸標籤
    k_values = data['k_value'].values
    xticks_labels = [f'2^{int(math.log2(k_value))}' for k_value in k_values]
    plt.xticks(range(len(k_values)), xticks_labels, rotation=45)

    plt.legend()
    plt.grid(True)
    
    # 儲存並顯示圖表
    plt.tight_layout()
    plt.savefig(output_image)
    plt.show()

def predict_missing_points(csv_file):
    # 讀取資料
    data = pd.read_csv(csv_file)
    
    # 確保有足夠的資料來做預測
    if len(data) < 2:
        return None
    
    # 取出 n 和 time 欄位
    X = data['k_value'].values.reshape(-1, 1)
    y = data['time'].values
    
    # 使用線性回歸來預測
    model = LinearRegression()
    model.fit(X, y)

    return model

def main():

    # 定義排序程式的可執行檔路徑
    cpp_programs = {
        "quick_lomuto": "./quick_lomuto",
        "quick_hoare": "./quick_hoare",
        "quick_3way": "./quick_3way"
    }

    output_files = {
        "quick_lomuto": "./output_8_quick_lomuto",
        "quick_hoare": "./output_8_quick_hoare",
        "quick_3way": "./output_8_quick_3way"
    }

    # 紀錄哪些資料點超時並需要預測
    missing_points = {
        "quick_lomuto": {"k_value": [], "predicted_time": []},
        "quick_hoare": {"k_value": [], "predicted_time": []},
        "quick_3way": {"k_value": [], "predicted_time": []}
    }

    # 記錄哪些資料結構已經跳過剩餘計算
    skipped_structures = {
        "quick_lomuto": False,
        "quick_hoare": False,
        "quick_3way": False
    }
    
    # 初始化 CSV 檔案
    for file in output_files.values():
        with open(file, 'w') as f:
            f.write("k_value,time\n")
    
    # 生成不同大小的數組並執行排序
    for k in range(0, 21 ):  # 可調整範圍
        size = 2 ** 15
        k_value = 2 ** k
        print(f"Running tests for k_value = {k_value}")

        for name, executable in cpp_programs.items():
            # 如果該資料結構已被標記為跳過，則跳過後續執行
            if skipped_structures[name]:
                print(f"Skipping {name} for k_value {k_value}")
                missing_points[name]["k_value"].append(k_value)
                continue

            success = run_cpp_program(executable, size, "third", k_value, output_files[name], timeout=3000)
            if not success:
                print(f"Skipping remaining points for {name} after k_value {k_value}")
                missing_points[name]["k_value"].append(k_value)
                skipped_structures[name] = True  # 標記此資料結構為跳過

    # 預測缺失資料點並將預測值加入 CSV
    for name, output_file in output_files.items():
        model = predict_missing_points(output_file)
        if model and missing_points[name]["k_value"]:
            for k_value in missing_points[name]["k_value"]:
                predicted_time = model.predict(np.array([[k_value]]))[0]
                with open(output_file, 'a') as f:
                    f.write(f"{k_value},{predicted_time}\n")
                missing_points[name]["predicted_time"].append(predicted_time)

    # 繪製圖表
    plot_graph(list(output_files.values()), list(cpp_programs.keys()), "graph_8.png", missing_points)

if __name__ == "__main__":
    main()
