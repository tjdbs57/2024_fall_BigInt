import matplotlib.pyplot as plt
import pandas as pd

# CSV 파일에서 성능 결과 읽기
data = pd.read_csv('performance_results.csv')

# 실행 시간 그래프
plt.figure(figsize=(10, 6))
plt.bar(data['Algorithm'], data['Execution Time (s)'], color='b', alpha=0.7)
plt.xlabel('Algorithm')
plt.ylabel('Execution Time (s)')
plt.title('Execution Time Comparison')
plt.show()

# CPU 클럭 사이클 그래프
plt.figure(figsize=(10, 6))
plt.bar(data['Algorithm'], data['CPU Cycles'], color='g', alpha=0.7)
plt.xlabel('Algorithm')
plt.ylabel('CPU Cycles')
plt.title('CPU Cycles Comparison')
plt.show()

# 메모리 사용량 그래프
plt.figure(figsize=(10, 6))
plt.bar(data['Algorithm'], data['Memory Usage (KB)'], color='r', alpha=0.7)
plt.xlabel('Algorithm')
plt.ylabel('Memory Usage (KB)')
plt.title('Memory Usage Comparison')
plt.show()