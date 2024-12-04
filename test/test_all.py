import subprocess
import matplotlib.pyplot as plt
import os

def process_test_output(output):
    lines = output.splitlines()
    results = {
        'add': {'success': 0, 'failure': 0},
        'sub': {'success': 0, 'failure': 0},
        'mul_tx': {'success': 0, 'failure': 0},
        'mul_improved': {'success': 0, 'failure': 0},
        'mul_karatsuba': {'success': 0, 'failure': 0},
        'div': {'success': 0, 'failure': 0},
        'L2R': {'success': 0, 'failure': 0},
        'R2L': {'success': 0, 'failure': 0},
        'exp_mod_montgomery': {'success': 0, 'failure': 0},
        'reduction': {'success': 0, 'failure': 0},
        'squ_tx': {'success': 0, 'failure': 0},
        'squ_karatsuba': {'success': 0, 'failure': 0},
    }

    for line in lines:
        if line.strip():  # 공백 라인 무시
            operator_and_expression = line.split(":")
            if len(operator_and_expression) == 2:
                operator = operator_and_expression[0].strip()
                expression = operator_and_expression[1].strip()

                parts = expression.split("==")
                if len(parts) == 2:
                    left = parts[0].strip()
                    right = parts[1].strip()
                    left_value = eval(left)
                    right_value = eval(right)

                    # 연산 종류 매핑
                    if operator in results:
                        operation = operator
                    else:
                        continue  # 알 수 없는 연산은 무시

                    # 결과 비교
                    if left_value == right_value:
                        results[operation]['success'] += 1
                    else:
                        results[operation]['failure'] += 1

    return results


def run_c_program():
    try:
        result = subprocess.run(['./bin/program'], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

        if result.returncode != 0:
            print(f"Error running C program: {result.stderr}")
            return {}

        return process_test_output(result.stdout)

    except Exception as e:
        print(f"Exception while running C program: {e}")
        return {}


def plot_success_rate(results, output_path, test_cases, bit_length):
    categories = list(results.keys())
    success_values = [results[operation]['success'] for operation in categories]
    failure_values = [results[operation]['failure'] for operation in categories]

    total_success = sum(success_values)
    total_failure = sum(failure_values)
    total_operations = total_success + total_failure

    success_rate = (total_success / total_operations) * 100 if total_operations > 0 else 0
    print(f"Success Rate: {success_rate:.2f}%")

    # 그래프 생성
    fig, ax = plt.subplots(figsize=(14, 8))  # 그래프 크기 조정

    bar_width = 0.35
    index = range(len(categories))

    bar1 = ax.bar(index, success_values, bar_width, label='Success', color='green')
    bar2 = ax.bar([i + bar_width for i in index], failure_values, bar_width, label='Failure', color='red')

    # 제목 설정에 테스트 케이스와 비트 길이 추가
    ax.set_title(
        f"Success and Failure Counts (Success Rate: {success_rate:.2f}%)\n"
        f"Test cases: {test_cases}, Bit length: {bit_length}"
    )
    ax.set_xlabel('Operations')
    ax.set_ylabel('Count')
    ax.set_xticks([i + bar_width / 2 for i in index])
    ax.set_xticklabels(categories, rotation=45, ha='right')  # 회전으로 연산자 표시
    ax.legend()

    plt.ylim(0, max(max(success_values), max(failure_values)) + 1)
    plt.grid(axis='y', linestyle='--', alpha=0.7)

    # 값 표시
    for i in range(len(categories)):
        ax.text(i, success_values[i] + 0.2, str(success_values[i]), ha='center', va='bottom', color='black')
        ax.text(i + bar_width, failure_values[i] + 0.2, str(failure_values[i]), ha='center', va='bottom', color='black')

    plt.tight_layout()

    # 그래프를 파일로 저장
    os.makedirs(os.path.dirname(output_path), exist_ok=True)  # 디렉토리 생성
    plt.savefig(output_path)
    print(f"Graph saved to {output_path}")
    plt.close()


# C 프로그램 실행 후 결과 처리
results = run_c_program()
if results:
    output_file = "graph/success_rate_plot.png"  # 저장될 그래프 파일 경로
    test_cases = 1000  # 테스트 케이스 개수
    bit_length = 4096  # 비트 길이
    plot_success_rate(results, output_file, test_cases, bit_length)
