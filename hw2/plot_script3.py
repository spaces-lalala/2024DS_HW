import os
import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import math
from sklearn.linear_model import LinearRegression
import numpy as np
 
# 執行 C++ 排序程式並生成 CSV
def run_cpp_program(executable_path, n_value, mode, k_value, s_value, output_file, timeout=3000):
    try:
        with open(output_file, 'a') as output:
            subprocess.run([executable_path, str(n_value), mode, str(k_value), str(s_value)], stdout=output, timeout=timeout)
        return True
    except subprocess.TimeoutExpired:
        print(f"Test for n = {n_value} timed out!")
        return False

# 繪製圖一的折線圖
def plot_graph(filenames, labels, output_image, missing_points):
    plt.figure(figsize=(10, 6))
    
    # 建立顏色映射，確保每個排序方法的預測點和線條顏色一致
    colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', 
          '#9467bd', '#8c564b', '#e377c2', '#7f7f7f', 
          '#bcbd22', '#17becf']  # 默認顏色循環
    
    for (filename, label), color in zip(zip(filenames, labels), colors):
        # 讀取 CSV 資料
        data = pd.read_csv(filename)
        x_ticks = range(len(data['size']))
        
        # 先畫折線和點
        plt.plot(x_ticks, data['time'], label=label, marker='o', color=color)
        
        # 再畫預測點（x標記），這樣確保x會在o上面
        if label in missing_points:
            missing_data = missing_points[label]
            missing_x_ticks = [data[data['size'] == size].index[0] for size in missing_data['size']]
            plt.scatter(missing_x_ticks, missing_data['predicted_time'], 
                       color=color, marker='x', s=100, zorder=5)  # 增加 zorder 確保在其他元素上面

    # 設置圖表的標籤和標題
    plt.xlabel("Array size (n)")
    plt.ylabel("Time to sort (seconds)")
    plt.title("Graph3")

    # 設定 X 軸標籤
    sizes = data['size'].values
    xticks_labels = [f'2^{int(math.log2(size))}' for size in sizes]
    plt.xticks(range(len(sizes)), xticks_labels, rotation=45)

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
    X = data['size'].values.reshape(-1, 1)
    y = data['time'].values
    
    # 使用線性回歸來預測
    model = LinearRegression()
    model.fit(X, y)

    return model

def main():

    # 定義排序程式的可執行檔路徑
    cpp_programs = {
        "quick_merge_1": "./quick_merge",
        "merge_quick_1": "./merge_quick",
        "quick_merge_2": "./quick_merge",
        "merge_quick_2": "./merge_quick",
        "quick_merge_3": "./quick_merge",
        "merge_quick_3": "./merge_quick",
        "quick_merge_4": "./quick_merge",
        "merge_quick_4": "./merge_quick",
        "quick_merge_5": "./quick_merge",
        "merge_quick_5": "./merge_quick",
    }

    output_files = {
        "quick_merge_1": "./output_3_quick_merge_1",
        "merge_quick_1": "./output_3_merge_quick_1",
        "quick_merge_2": "./output_3_quick_merge_2",
        "merge_quick_2": "./output_3_merge_quick_2",
        "quick_merge_3": "./output_3_quick_merge_3",
        "merge_quick_3": "./output_3_merge_quick_3",
        "quick_merge_4": "./output_3_quick_merge_4",
        "merge_quick_4": "./output_3_merge_quick_4",
        "quick_merge_5": "./output_3_quick_merge_5",
        "merge_quick_5": "./output_3_merge_quick_5"
    }

    # 紀錄哪些資料點超時並需要預測
    missing_points = {
        "quick_merge_1": {"size": [], "predicted_time": []},
        "merge_quick_1": {"size": [], "predicted_time": []},
        "quick_merge_2": {"size": [], "predicted_time": []},
        "merge_quick_2": {"size": [], "predicted_time": []},
        "quick_merge_3": {"size": [], "predicted_time": []},
        "merge_quick_3": {"size": [], "predicted_time": []},
        "quick_merge_4": {"size": [], "predicted_time": []},
        "merge_quick_4": {"size": [], "predicted_time": []},
        "quick_merge_5": {"size": [], "predicted_time": []},
        "merge_quick_5": {"size": [], "predicted_time": []},
    }

    # 記錄哪些資料結構已經跳過剩餘計算
    skipped_structures = {
        "quick_merge_1": False,
        "merge_quick_1": False,
        "quick_merge_2": False,
        "merge_quick_2": False,
        "quick_merge_3": False,
        "merge_quick_3": False,
        "quick_merge_4": False,
        "merge_quick_4": False,
        "quick_merge_5": False,
        "merge_quick_5": False,
    }
    
    # 初始化 CSV 檔案
    for file in output_files.values():
        with open(file, 'w') as f:
            f.write("size,time\n")
    
    # 生成不同大小的數組並執行排序
    for s in range(1,6):
        s_value = s
        for n in range(10, 31):  # 可調整範圍
            size = 2 ** n
            print(f"Running tests for array size = {size}")

            if n <26:
                for name, executable in cpp_programs.items():
                    # 如果該資料結構已被標記為跳過，則跳過後續執行

                    if not name.endswith(str(s)):
                        continue  # 如果不匹配，則跳過此迭代

                    if skipped_structures[name]:
                        print(f"Skipping {name} for size {size}")
                        missing_points[name]["size"].append(size)
                        continue

                    success = run_cpp_program(executable, size, "first", 0,s_value, output_files[name], timeout=3000)
                    if not success:
                        print(f"Skipping remaining points for {name} after size {size}")
                        missing_points[name]["size"].append(size)
                        skipped_structures[name] = True  # 標記此資料結構為跳過
            else:
                for name, executable in cpp_programs.items():
                    if not name.endswith(str(s)):
                        continue  # 如果不匹配，則跳過此迭代
                    print(f"Skipping {name} for size {size}")
                    missing_points[name]["size"].append(size)
                    continue


        # 預測缺失資料點並將預測值加入 CSV
    for name, output_file in output_files.items():
        model = predict_missing_points(output_file)
        if model and missing_points[name]["size"]:
            for size in missing_points[name]["size"]:
                predicted_time = model.predict(np.array([[size]]))[0]
                with open(output_file, 'a') as f:
                    f.write(f"{size},{predicted_time}\n")
                missing_points[name]["predicted_time"].append(predicted_time)

    # 繪製圖表
    plot_graph(list(output_files.values()), list(cpp_programs.keys()), "graph_3.png", missing_points)

if __name__ == "__main__":
    main()
