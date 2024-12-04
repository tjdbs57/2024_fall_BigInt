import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# 파일 읽기
try:
    with open('cycle_single.txt', 'r', encoding='utf-8') as file:
        data = file.readlines()
    function1_speeds = [float(line.strip()) for line in data]
except FileNotFoundError:
    print("Error: The file 'cycle_single.txt' was not found.")
    exit()
except ValueError:
    print("Error: Could not convert some lines in the file to float.")
    exit()

# 평균 계산
average_function1 = np.mean(function1_speeds)

# Rolling mean 계산 (window_size 조정)
window_size = 100
if len(function1_speeds) < window_size:
    print(f"Warning: Window size ({window_size}) is larger than the data size ({len(function1_speeds)}). Adjusting window size.")
    window_size = max(1, len(function1_speeds) // 10)  # 데이터 크기에 맞게 window_size를 조정

function1_smooth = pd.Series(function1_speeds).rolling(window=window_size).mean()

# NaN 제거
function1_smooth_no_nan = function1_smooth.dropna()

# 최소 및 최대값 계산 (NaN이 아닌 경우에만)
if not function1_smooth_no_nan.empty:
    lowest_point_smooth = function1_smooth_no_nan.min()
    highest_point_smooth = function1_smooth_no_nan.max()
    lowest_index_smooth = function1_smooth_no_nan.idxmin()
    highest_index_smooth = function1_smooth_no_nan.idxmax()
else:
    print("Error: Rolling mean resulted in an empty sequence. Check your window size and data.")
    exit()

# 그래프 설정
fig, ax1 = plt.subplots(figsize=(14, 7), dpi=100)

# Smoothed data plot
ax1.plot(function1_smooth, color='dodgerblue', label='Smoothed Cycles', alpha=0.9, linewidth=2, linestyle='-')

# 원본 데이터 (옵션: 너무 길다면 주석 처리)
ax1.plot(function1_speeds, alpha=0.3, color='grey', label='Original Data')

# 최소 및 최대 포인트 표시
ax1.plot(lowest_index_smooth, lowest_point_smooth, 'v', color='green', markersize=10)
ax1.text(lowest_index_smooth, lowest_point_smooth, f'Lowest: {lowest_point_smooth:.2f}', color='green', va='top', ha='center')

ax1.plot(highest_index_smooth, highest_point_smooth, '^', color='red', markersize=10)
ax1.text(highest_index_smooth, highest_point_smooth, f'Highest: {highest_point_smooth:.2f}', color='red', va='bottom', ha='center')

# 평균 라인 추가
ax1.axhline(average_function1, color='orange', linestyle='dashdot', linewidth=2, label=f'Average Cycles: {average_function1:.2f}')

# 제목 및 레이블
ax1.set_ylabel('Total Cycles', fontsize=16, fontweight='bold', color='dodgerblue')
ax1.set_xlabel('Run Number', fontsize=16, fontweight='bold')
ax1.set_title('Measurement of Cycles: 32-bit addition', fontsize=18, fontweight='bold')

# 범례 설정
lines, labels = ax1.get_legend_handles_labels()
ax1.legend(lines, labels, loc='upper left', frameon=True, shadow=True, fancybox=True, fontsize=12)

# 그리드 및 레이아웃
ax1.grid(True, linestyle='--', linewidth=0.5, alpha=0.7)
plt.tight_layout()

# 플롯 디자인
fig.patch.set_facecolor('white')

# 그래프 표시
plt.show()