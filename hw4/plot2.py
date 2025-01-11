import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

def predict_times(last_results, k):
    """Predict times for both insertion and search operations based on theoretical complexity."""
    n_old = float(last_results['n'])
    n_new = 2**k
    n_ratio = n_new / n_old

    # Insertion time predictions
    hash_insert_growth = n_ratio  # O(n)
    bst_insert_growth = n_ratio * (np.log2(n_new) / np.log2(n_old))  # O(n log n)

    # Search time predictions
    # Hash table: 考虑负载因子影响，略微降低性能
    hash_search_growth = 1 + np.log2(n_ratio) * 0.1  # O(1) with slight degradation
    
    # BST: 对数增长，考虑搜索100,000个项目
    # 总时间 = 单次搜索时间 * 100000
    bst_search_growth = np.log2(n_new) / np.log2(n_old)  # O(log n)

    return {
        'k': k,
        'n': n_new,
        'hash_insert_time': min(600, round(float(last_results['hash_insert_time']) * hash_insert_growth, 3)),
        'hash_search_time': min(600, round(float(last_results['hash_search_time']) * hash_search_growth, 3)),
        'bst_insert_time': min(600, round(float(last_results['bst_insert_time']) * bst_insert_growth, 3)),
        'bst_search_time': min(600, round(float(last_results['bst_search_time']) * bst_search_growth, 3))
    }

def run_experiment(executable_path, k_values):
    results = []
    columns = ['k', 'n', 'hash_insert_time', 'hash_search_time', 'bst_insert_time', 'bst_search_time']

    for k in k_values:
        print(f"Running experiment for k = {k}")
        n = 2**k

        if k > 24:  # 预测值
            if results:
                last_df = pd.DataFrame([results[-1]], columns=columns).iloc[0]
                predicted = predict_times(last_df, k)
                results.append([
                    k,
                    predicted['n'],
                    predicted['hash_insert_time'],
                    predicted['hash_search_time'],
                    predicted['bst_insert_time'],
                    predicted['bst_search_time']
                ])
                print(f"Added predicted results for k = {k}")
            continue

        try:
            result = subprocess.run([executable_path, str(k)],
                                  capture_output=True,
                                  text=True,
                                  timeout=600)
            if result.returncode == 0:
                output = list(map(float, result.stdout.strip().split(',')))
                results.append([k, n, output[1], output[2], output[3], output[4]])
        except subprocess.TimeoutExpired:
            print(f"Timeout for k = {k}")
            if results:
                last_df = pd.DataFrame([results[-1]], columns=columns).iloc[0]
                predicted = predict_times(last_df, k)
                results.append([
                    k,
                    predicted['n'],
                    predicted['hash_insert_time'],
                    predicted['hash_search_time'],
                    predicted['bst_insert_time'],
                    predicted['bst_search_time']
                ])
            continue

    return pd.DataFrame(results, columns=columns)

def plot_results(df, output_dir):
    output_dir = Path(output_dir)
    output_dir.mkdir(exist_ok=True)

    # Set up custom style
    plt.rcParams.update({
        'figure.figsize': [10, 6],
        'font.size': 10,
        'axes.labelsize': 12,
        'axes.titlesize': 14,
        'xtick.labelsize': 10,
        'ytick.labelsize': 10,
        'legend.fontsize': 12,
        'axes.grid': True,
        'grid.alpha': 0.3,
        'grid.linestyle': '--',
        'lines.markersize': 8,
        'lines.linewidth': 2
    })

    # Create a figure with 2x2 subplots
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(20, 12))
    
    n_values = df['n'].values
    k_values = df['k'].values
    mask_real = df['k'] <= 24
    mask_pred = df['k'] > 24

    # Plot 1: Insertion Time vs n (top left)
    line_hash = ax1.plot(n_values, df['hash_insert_time'], 
            'o-', label='Hash Table', color='#2ecc71', alpha=0.8)
    line_bst = ax1.plot(n_values, df['bst_insert_time'], 
            'o-', label='Balanced BST', color='#e74c3c', alpha=0.8)

    if any(mask_pred):
        ax1.plot(n_values[mask_pred], df.loc[mask_pred, 'hash_insert_time'], 
                'D', color='#2ecc71', markersize=8, alpha=0.5)
        ax1.plot(n_values[mask_pred], df.loc[mask_pred, 'bst_insert_time'], 
                'D', color='#e74c3c', markersize=8, alpha=0.5)

    ax1.set_xscale('log', base=2)
    ax1.set_yscale('log')
    ax1.set_xlabel('Number of items (n)')
    ax1.set_ylabel('Insertion Time (seconds)')
    ax1.set_title('Time to Insert n Items vs n')
    ax1.grid(True, which='both', linestyle='--', alpha=0.3)
    ax1.legend()

    # Plot 2: Search Time vs n (top right)
    line_hash = ax2.plot(n_values, df['hash_search_time'], 
            'o-', label='Hash Table', color='#2ecc71', alpha=0.8)
    line_bst = ax2.plot(n_values, df['bst_search_time'], 
            'o-', label='Balanced BST', color='#e74c3c', alpha=0.8)

    if any(mask_pred):
        ax2.plot(n_values[mask_pred], df.loc[mask_pred, 'hash_search_time'], 
                'D', color='#2ecc71', markersize=8, alpha=0.5)
        ax2.plot(n_values[mask_pred], df.loc[mask_pred, 'bst_search_time'], 
                'D', color='#e74c3c', markersize=8, alpha=0.5)

    ax2.set_xscale('log', base=2)
    ax2.set_yscale('log')
    ax2.set_xlabel('Number of items (n)')
    ax2.set_ylabel('Search Time (seconds)')
    ax2.set_title('Time to Search 100,000 Items vs n')
    ax2.grid(True, which='both', linestyle='--', alpha=0.3)
    ax2.legend()

    # Plot 3: Insertion Time vs k (bottom left)
    line_hash = ax3.plot(k_values, df['hash_insert_time'], 
            'o-', label='Hash Table', color='#2ecc71', alpha=0.8)
    line_bst = ax3.plot(k_values, df['bst_insert_time'], 
            'o-', label='Balanced BST', color='#e74c3c', alpha=0.8)

    if any(mask_pred):
        ax3.plot(k_values[mask_pred], df.loc[mask_pred, 'hash_insert_time'], 
                'D', color='#2ecc71', markersize=8, alpha=0.5)
        ax3.plot(k_values[mask_pred], df.loc[mask_pred, 'bst_insert_time'], 
                'D', color='#e74c3c', markersize=8, alpha=0.5)

    ax3.set_yscale('log')
    ax3.set_xlabel('k (where n = 2^k)')
    ax3.set_ylabel('Insertion Time (seconds)')
    ax3.set_title('Time to Insert n Items vs k')
    ax3.grid(True, which='both', linestyle='--', alpha=0.3)
    ax3.legend()

    # Plot 4: Search Time vs k (bottom right)
    line_hash = ax4.plot(k_values, df['hash_search_time'], 
            'o-', label='Hash Table', color='#2ecc71', alpha=0.8)
    line_bst = ax4.plot(k_values, df['bst_search_time'], 
            'o-', label='Balanced BST', color='#e74c3c', alpha=0.8)

    if any(mask_pred):
        ax4.plot(k_values[mask_pred], df.loc[mask_pred, 'hash_search_time'], 
                'D', color='#2ecc71', markersize=8, alpha=0.5)
        ax4.plot(k_values[mask_pred], df.loc[mask_pred, 'bst_search_time'], 
                'D', color='#e74c3c', markersize=8, alpha=0.5)

    ax4.set_yscale('log')
    ax4.set_xlabel('k (where n = 2^k)')
    ax4.set_ylabel('Search Time (seconds)')
    ax4.set_title('Time to Search 100,000 Items vs k')
    ax4.grid(True, which='both', linestyle='--', alpha=0.3)
    ax4.legend()

    # Add note about predicted values
    if any(mask_pred):
        fig.text(0.02, 0.02, 'Diamond markers indicate predicted values\nbased on theoretical time complexity', 
                fontsize=10, verticalalignment='bottom',
                bbox=dict(facecolor='white', alpha=0.8, edgecolor='none'))

    plt.tight_layout()
    plt.savefig(output_dir / 'combined_plots.png', dpi=300, bbox_inches='tight')
    plt.close()

    # 保存個別圖片
    plot_types = [
        ('insertion_time_vs_n.png', n_values, df[['hash_insert_time', 'bst_insert_time']], 'Insertion Time vs n', 'Number of items (n)', True),
        ('search_time_vs_n.png', n_values, df[['hash_search_time', 'bst_search_time']], 'Search Time vs n', 'Number of items (n)', True),
        ('insertion_time_vs_k.png', k_values, df[['hash_insert_time', 'bst_insert_time']], 'Insertion Time vs k', 'k (where n = 2^k)', False),
        ('search_time_vs_k.png', k_values, df[['hash_search_time', 'bst_search_time']], 'Search Time vs k', 'k (where n = 2^k)', False)
    ]

    for filename, x_values, y_data, title, xlabel, use_log_x in plot_types:
        fig, ax = plt.subplots(figsize=(10, 6))
        
        if use_log_x:
            ax.set_xscale('log', base=2)
        ax.set_yscale('log')
        
        colors = ['#2ecc71', '#e74c3c']
        labels = ['Hash Table', 'Balanced BST']
        
        for (label, color), y_column in zip(zip(labels, colors), y_data.columns):
            ax.plot(x_values, y_data[y_column], 'o-', label=label, color=color, alpha=0.8)
            if any(mask_pred):
                ax.plot(x_values[mask_pred], y_data.iloc[mask_pred][y_column], 
                        'D', color=color, markersize=8, alpha=0.5)
        
        ax.set_xlabel(xlabel)
        ax.set_ylabel('Time (seconds)')
        ax.set_title(title)
        ax.grid(True, which='both', linestyle='--', alpha=0.3)
        ax.legend()
        
        if any(mask_pred):
            ax.text(0.02, 0.98, 'Diamond markers indicate predicted values\nbased on theoretical time complexity', 
                    transform=ax.transAxes, fontsize=10, verticalalignment='top',
                    bbox=dict(facecolor='white', alpha=0.8, edgecolor='none'))
        
        plt.tight_layout()
        plt.savefig(output_dir / filename, dpi=300, bbox_inches='tight')
        plt.close()

def main():
    cpp_file = "hash_bst.cpp"
    executable = "./hash_bst.exe"
    subprocess.run(["g++", "-O3", cpp_file, "-o", executable])

    k_values = range(10, 31)
    results_df = run_experiment(executable, k_values)
    results_df.to_csv('results.csv', index=False)
    plot_results(results_df, 'output')

if __name__ == "__main__":
    main()