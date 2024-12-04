import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# 새로운 연산 종류에 대한 데이터 리스트
mul_core_tx_speeds = []
mul_core_improved_speeds = []
mul_core_karatsuba_speeds = []

# 데이터 파일 읽기 (각 연산 종류에 대해 번갈아 읽기)
try:
    with open('cycle.txt', 'r', encoding='utf-8') as file:
        for line_number, line in enumerate(file):
            line = line.strip()
            if line_number % 3 == 0:  # 첫 번째 줄은 mul_core_tx
                mul_core_tx_speeds.append(float(line))
            elif line_number % 3 == 1:  # 두 번째 줄은 mul_core_improved
                mul_core_improved_speeds.append(float(line))
            else:  # 세 번째 줄은 mul_core_karatsuba
                mul_core_karatsuba_speeds.append(float(line))
except FileNotFoundError:
    print("Error: The file 'cycle.txt' was not found.")
    exit()
except ValueError:
    print("Error: Could not convert some lines in the file to float.")
    exit()

# 평균 계산
average_mul_core_tx = np.mean(mul_core_tx_speeds)
average_mul_core_improved = np.mean(mul_core_improved_speeds)
average_mul_core_karatsuba = np.mean(mul_core_karatsuba_speeds)

# Rolling mean 계산 (윈도우 크기 조정)
window_size = 100
if len(mul_core_tx_speeds) < window_size:
    print(f"Warning: Window size ({window_size}) is larger than the data size ({len(mul_core_tx_speeds)}). Adjusting window size.")
    window_size = max(1, len(mul_core_tx_speeds) // 10)  # 데이터 크기에 맞게 window_size를 조정

# 각 연산 종류에 대해 Rolling mean 계산
mul_core_tx_smooth = pd.Series(mul_core_tx_speeds).rolling(window=window_size).mean()
mul_core_improved_smooth = pd.Series(mul_core_improved_speeds).rolling(window=window_size).mean()
mul_core_karatsuba_smooth = pd.Series(mul_core_karatsuba_speeds).rolling(window=window_size).mean()

# NaN 제거
mul_core_tx_smooth_no_nan = mul_core_tx_smooth.dropna()
mul_core_improved_smooth_no_nan = mul_core_improved_smooth.dropna()
mul_core_karatsuba_smooth_no_nan = mul_core_karatsuba_smooth.dropna()

# 최소 및 최대값 계산 (NaN이 아닌 경우에만)
if not mul_core_tx_smooth_no_nan.empty:
    lowest_mul_core_tx = mul_core_tx_smooth_no_nan.min()
    highest_mul_core_tx = mul_core_tx_smooth_no_nan.max()
    lowest_index_mul_core_tx = mul_core_tx_smooth_no_nan.idxmin()
    highest_index_mul_core_tx = mul_core_tx_smooth_no_nan.idxmax()

if not mul_core_improved_smooth_no_nan.empty:
    lowest_mul_core_improved = mul_core_improved_smooth_no_nan.min()
    highest_mul_core_improved = mul_core_improved_smooth_no_nan.max()
    lowest_index_mul_core_improved = mul_core_improved_smooth_no_nan.idxmin()
    highest_index_mul_core_improved = mul_core_improved_smooth_no_nan.idxmax()

if not mul_core_karatsuba_smooth_no_nan.empty:
    lowest_mul_core_karatsuba = mul_core_karatsuba_smooth_no_nan.min()
    highest_mul_core_karatsuba = mul_core_karatsuba_smooth_no_nan.max()
    lowest_index_mul_core_karatsuba = mul_core_karatsuba_smooth_no_nan.idxmin()
    highest_index_mul_core_karatsuba = mul_core_karatsuba_smooth_no_nan.idxmax()

# 그래프 설정
fig, ax1 = plt.subplots(figsize=(14, 7), dpi=100)

# 각 연산 종류에 대해 롤링 평균 그래프 그리기
ax1.plot(mul_core_tx_smooth_no_nan, color='dodgerblue', label='Mul Core TX Cycles', alpha=0.9, linewidth=2, linestyle='-')
ax1.plot(mul_core_improved_smooth_no_nan, color='tomato', label='Mul Core Improved Cycles', alpha=0.9, linewidth=2, linestyle='-')
ax1.plot(mul_core_karatsuba_smooth_no_nan, color='green', label='Mul Core Karatsuba Cycles', alpha=0.9, linewidth=2, linestyle='-')

# 각 연산의 최소 및 최대 포인트 표시
ax1.plot(lowest_index_mul_core_tx, lowest_mul_core_tx, 'v', color='green', markersize=10)
ax1.text(lowest_index_mul_core_tx, lowest_mul_core_tx, f'Lowest: {lowest_mul_core_tx:.2f}', color='green', va='top', ha='center')

ax1.plot(highest_index_mul_core_tx, highest_mul_core_tx, '^', color='red', markersize=10)
ax1.text(highest_index_mul_core_tx, highest_mul_core_tx, f'Highest: {highest_mul_core_tx:.2f}', color='red', va='bottom', ha='center')

ax1.plot(lowest_index_mul_core_improved, lowest_mul_core_improved, 'v', color='purple', markersize=10)
ax1.text(lowest_index_mul_core_improved, lowest_mul_core_improved, f'Lowest: {lowest_mul_core_improved:.2f}', color='purple', va='top', ha='center')

ax1.plot(highest_index_mul_core_improved, highest_mul_core_improved, '^', color='orange', markersize=10)
ax1.text(highest_index_mul_core_improved, highest_mul_core_improved, f'Highest: {highest_mul_core_improved:.2f}', color='orange', va='bottom', ha='center')

ax1.plot(lowest_index_mul_core_karatsuba, lowest_mul_core_karatsuba, 'v', color='blue', markersize=10)
ax1.text(lowest_index_mul_core_karatsuba, lowest_mul_core_karatsuba, f'Lowest: {lowest_mul_core_karatsuba:.2f}', color='blue', va='top', ha='center')

ax1.plot(highest_index_mul_core_karatsuba, highest_mul_core_karatsuba, '^', color='yellow', markersize=10)
ax1.text(highest_index_mul_core_karatsuba, highest_mul_core_karatsuba, f'Highest: {highest_mul_core_karatsuba:.2f}', color='yellow', va='bottom', ha='center')

# 평균 라인 추가
ax1.axhline(average_mul_core_tx, color='dodgerblue', linestyle='dashdot', linewidth=2, label=f'Avg Mul Core TX Cycles: {average_mul_core_tx:.2f}')
ax1.axhline(average_mul_core_improved, color='tomato', linestyle='dashdot', linewidth=2, label=f'Avg Mul Core Improved Cycles: {average_mul_core_improved:.2f}')
ax1.axhline(average_mul_core_karatsuba, color='green', linestyle='dashdot', linewidth=2, label=f'Avg Mul Core Karatsuba Cycles: {average_mul_core_karatsuba:.2f}')

# 제목 및 레이블
ax1.set_ylabel('Total Cycles', fontsize=16, fontweight='bold', color='dodgerblue')
ax1.set_xlabel('Run Number', fontsize=16, fontweight='bold')
#ax1.set_title('Measurement of Cycles: Mul Core TX, Improved, and Karatsuba', fontsize=18, fontweight='bold')
ax1.set_title(
    'Measurement of Cycles (64-bit Multiplication): Mul Core TX, Improved, and Karatsuba',
    fontsize=18,
    fontweight='bold'
)

# 범례 설정
#lines, labels = ax1.get_legend_handles_labels()
#ax1.legend(lines, labels, loc='upper left', frameon=True, shadow=True, fancybox=True, fontsize=12)
# 범례를 그래프 외부로 이동
lines, labels = ax1.get_legend_handles_labels()
ax1.legend(lines, labels, loc='upper center', bbox_to_anchor=(0.5, -0.15), 
           frameon=True, shadow=True, fancybox=True, fontsize=12, ncol=2)

# 그리드 및 레이아웃
ax1.grid(True, linestyle='--', linewidth=0.5, alpha=0.7)
plt.tight_layout(rect=[0, 0.2, 1, 1])  # 그래프 크기를 재조정해 하단 공간 확보

# 그리드 및 레이아웃
#ax1.grid(True, linestyle='--', linewidth=0.5, alpha=0.7)
#plt.tight_layout()

# 플롯 디자인
fig.patch.set_facecolor('white')

# 그래프 표시
plt.show()
