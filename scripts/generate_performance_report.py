#!/usr/bin/env python3
# File: scripts/generate_performance_report.py

import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

def calculate_metrics(df):
    """Calculate speedup and efficiency for each paradigm"""
    for paradigm in df['Paradigm'].unique():
        mask = df['Paradigm'] == paradigm
        baseline_time = df[mask & (df['Threads'] == 1)]['ExecutionTime(s)'].values[0]
        
        df.loc[mask, 'Speedup'] = baseline_time / df.loc[mask, 'ExecutionTime(s)']
        df.loc[mask, 'Efficiency'] = df.loc[mask, 'Speedup'] / df.loc[mask, 'Threads']
    
    return df

def generate_graphs(df, output_dir):
    """Generate comprehensive performance visualization graphs"""
    
    plt.style.use('seaborn-v0_8-darkgrid')
    
    # 1. Speedup Comparison
    fig, ax = plt.subplots(figsize=(10, 6))
    
    colors = {'std::thread': '#2E86AB', 'OpenMP': '#A23B72'}
    markers = {'std::thread': 'o', 'OpenMP': 's'}
    
    for paradigm in df['Paradigm'].unique():
        data = df[df['Paradigm'] == paradigm]
        ax.plot(data['Threads'], data['Speedup'], 
                marker=markers[paradigm], 
                label=f'C++ {paradigm}',
                linewidth=2.5,
                markersize=8,
                color=colors[paradigm])
    
    # Ideal speedup line
    max_threads = df['Threads'].max()
    ax.plot([1, max_threads], [1, max_threads], 
            'k--', label='Ideal Speedup', alpha=0.5, linewidth=2)
    
    ax.set_xlabel('Number of Threads', fontsize=12, fontweight='bold')
    ax.set_ylabel('Speedup', fontsize=12, fontweight='bold')
    ax.set_title('Speedup Comparison: std::thread vs OpenMP', 
                 fontsize=14, fontweight='bold')
    ax.legend(fontsize=10)
    ax.grid(True, alpha=0.3)
    ax.set_xticks(df['Threads'].unique())
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/speedup_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 2. Efficiency Comparison
    fig, ax = plt.subplots(figsize=(10, 6))
    
    for paradigm in df['Paradigm'].unique():
        data = df[df['Paradigm'] == paradigm]
        ax.plot(data['Threads'], data['Efficiency'] * 100, 
                marker=markers[paradigm],
                label=f'C++ {paradigm}',
                linewidth=2.5,
                markersize=8,
                color=colors[paradigm])
    
    ax.axhline(y=100, color='k', linestyle='--', alpha=0.5, 
               linewidth=2, label='100% Efficiency')
    ax.set_xlabel('Number of Threads', fontsize=12, fontweight='bold')
    ax.set_ylabel('Parallel Efficiency (%)', fontsize=12, fontweight='bold')
    ax.set_title('Parallel Efficiency: std::thread vs OpenMP', 
                 fontsize=14, fontweight='bold')
    ax.legend(fontsize=10)
    ax.grid(True, alpha=0.3)
    ax.set_xticks(df['Threads'].unique())
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/efficiency_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 3. Execution Time Bar Chart
    fig, ax = plt.subplots(figsize=(12, 6))
    
    x = np.arange(len(df['Threads'].unique()))
    width = 0.35
    
    threads_data = df[df['Paradigm'] == 'std::thread']
    openmp_data = df[df['Paradigm'] == 'OpenMP']
    
    ax.bar(x - width/2, threads_data['ExecutionTime(s)'], width, 
           label='std::thread', color=colors['std::thread'], alpha=0.8)
    ax.bar(x + width/2, openmp_data['ExecutionTime(s)'], width,
           label='OpenMP', color=colors['OpenMP'], alpha=0.8)
    
    ax.set_xlabel('Number of Threads', fontsize=12, fontweight='bold')
    ax.set_ylabel('Execution Time (seconds)', fontsize=12, fontweight='bold')
    ax.set_title('Execution Time Comparison', fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels(threads_data['Threads'])
    ax.legend(fontsize=10)
    ax.grid(True, alpha=0.3, axis='y')
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/execution_time_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 4. Time per Image Analysis
    fig, ax = plt.subplots(figsize=(10, 6))
    
    for paradigm in df['Paradigm'].unique():
        data = df[df['Paradigm'] == paradigm]
        ax.plot(data['Threads'], data['TimePerImage(s)'], 
                marker=markers[paradigm],
                label=f'C++ {paradigm}',
                linewidth=2.5,
                markersize=8,
                color=colors[paradigm])
    
    ax.set_xlabel('Number of Threads', fontsize=12, fontweight='bold')
    ax.set_ylabel('Time per Image (seconds)', fontsize=12, fontweight='bold')
    ax.set_title('Processing Time per Image', fontsize=14, fontweight='bold')
    ax.legend(fontsize=10)
    ax.grid(True, alpha=0.3)
    ax.set_xticks(df['Threads'].unique())
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/time_per_image.png', dpi=300, bbox_inches='tight')
    plt.close()

def generate_markdown_report(df, output_file):
    """Generate detailed markdown performance report"""
    
    with open(output_file, 'w') as f:
        f.write("# Performance Analysis Report\n")
        f.write("## C++ Implementation: std::thread vs OpenMP\n\n")
        f.write(f"**Generated:** {pd.Timestamp.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
        
        f.write("---\n\n")
        f.write("## Complete Performance Data\n\n")
        f.write(df.to_markdown(index=False, floatfmt=".4f"))
        f.write("\n\n")
        
        f.write("---\n\n")
        f.write("## Performance Summary\n\n")
        
        for paradigm in ['std::thread', 'OpenMP']:
            data = df[df['Paradigm'] == paradigm]
            max_speedup = data['Speedup'].max()
            max_speedup_threads = data.loc[data['Speedup'].idxmax(), 'Threads']
            avg_efficiency = data['Efficiency'].mean() * 100
            
            best_time = data['ExecutionTime(s)'].min()
            best_time_threads = data.loc[data['ExecutionTime(s)'].idxmin(), 'Threads']
            
            f.write(f"### {paradigm}\n\n")
            f.write(f"- **Maximum Speedup:** {max_speedup:.2f}x at {int(max_speedup_threads)} threads\n")
            f.write(f"- **Average Efficiency:** {avg_efficiency:.2f}%\n")
            f.write(f"- **Best Execution Time:** {best_time:.2f}s at {int(best_time_threads)} threads\n")
            f.write(f"- **Images Processed:** {int(data['ImagesProcessed'].iloc[0])}\n")
            f.write(f"- **Best Time per Image:** {data['TimePerImage(s)'].min():.4f}s\n\n")
        
        f.write("---\n\n")
        f.write("## Comparative Analysis\n\n")
        
        # Compare at each thread count
        for threads in sorted(df['Threads'].unique()):
            thread_data = df[df['Threads'] == threads]
            if len(thread_data) >= 2:
                std_time = thread_data[thread_data['Paradigm'] == 'std::thread']['ExecutionTime(s)'].values[0]
                omp_time = thread_data[thread_data['Paradigm'] == 'OpenMP']['ExecutionTime(s)'].values[0]
                
                faster = 'OpenMP' if omp_time < std_time else 'std::thread'
                diff_pct = abs(std_time - omp_time) / max(std_time, omp_time) * 100
                
                f.write(f"**{threads} Thread(s):**\n")
                f.write(f"- std::thread: {std_time:.2f}s\n")
                f.write(f"- OpenMP: {omp_time:.2f}s\n")
                f.write(f"- **{faster}** is faster by {diff_pct:.1f}%\n\n")

def print_summary(df):
    """Print summary to console"""
    print("\n" + "="*60)
    print("PERFORMANCE SUMMARY")
    print("="*60)
    
    for paradigm in ['std::thread', 'OpenMP']:
        data = df[df['Paradigm'] == paradigm]
        print(f"\n{paradigm}:")
        print(f"  Max Speedup: {data['Speedup'].max():.2f}x")
        print(f"  Avg Efficiency: {data['Efficiency'].mean()*100:.1f}%")
        print(f"  Best Time: {data['ExecutionTime(s)'].min():.2f}s")

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 generate_performance_report.py <results_csv_file>")
        sys.exit(1)
    
    csv_file = sys.argv[1]
    output_dir = '../results'
    
    # Read data
    print(f"Reading data from {csv_file}...")
    df = pd.read_csv(csv_file)
    
    # Calculate metrics
    print("Calculating speedup and efficiency...")
    df = calculate_metrics(df)
    
    # Save updated CSV
    df.to_csv(csv_file, index=False)
    print(f"✓ Updated CSV saved: {csv_file}")
    
    # Generate visualizations
    print("Generating performance graphs...")
    generate_graphs(df, output_dir)
    print(f"✓ Graphs saved to {output_dir}/")
    
    # Generate markdown report
    report_file = f'{output_dir}/performance_report.md'
    print("Generating markdown report...")
    generate_markdown_report(df, report_file)
    print(f"✓ Report saved: {report_file}")
    
    # Print summary
    print_summary(df)
    
    print("\n" + "="*60)
    print("Performance analysis complete!")
    print("="*60)

if __name__ == "__main__":
    main()