import matplotlib.pyplot as plt
import os

# 테스트 출력 파일을 읽고 결과를 처리하는 함수
def process_test_output(output):
    """
    @brief Processes the output of a test file and calculates success and failure counts for each operation.
    @param output A string containing the content of the test file.
    @return A dictionary summarizing success and failure counts for each operation.
    """
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
                    left_value = eval(left)  # 왼쪽 표현식 평가
                    right_value = eval(right)  # 오른쪽 표현식 평가

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


def read_test_output_from_file(file_path):

    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            output = f.read()
        return process_test_output(output)
    except Exception as e:
        print(f"Exception while reading test file: {e}")
        return {}


def plot_success_rate(results, output_path, test_cases, bit_length):
    """
    @brief Plots the success and failure counts for each operation and saves the graph to a file.
    @param results A dictionary containing success and failure counts for each operation.
    @param output_path The file path to save the resulting graph.
    @param test_cases The number of test cases used in the operations.
    @param bit_length The bit length of the test cases.
    """
    categories = list(results.keys())  # 연산 종류 목록
    success_values = [results[operation]['success'] for operation in categories]  # 성공 값
    failure_values = [results[operation]['failure'] for operation in categories]  # 실패 값

    total_success = sum(success_values)
    total_failure = sum(failure_values)
    total_operations = total_success + total_failure

    success_rate = (total_success / total_operations) * 100 if total_operations > 0 else 0
    print(f"Success Rate: {success_rate:.2f}%")

    # 그래프 생성
    fig, ax = plt.subplots(figsize=(14, 8))  # 그래프 크기 조정

    bar_width = 0.35  # 막대 너비 설정
    index = range(len(categories))  # 카테고리 인덱스

    bar1 = ax.bar(index, success_values, bar_width, label='Success', color='green')  # 성공 막대
    bar2 = ax.bar([i + bar_width for i in index], failure_values, bar_width, label='Failure', color='red')  # 실패 막대

    # 제목 설정에 테스트 케이스와 비트 길이 추가
    ax.set_title(
        f"Success and Failure Counts (Success Rate: {success_rate:.2f}%)\n"
        f"Test cases: {test_cases}, Bit length: {bit_length}"
    )
    ax.set_xlabel('Operations')  # X축 레이블
    ax.set_ylabel('Count')  # Y축 레이블
    ax.set_xticks([i + bar_width / 2 for i in index])  # X축 눈금 위치
    ax.set_xticklabels(categories, rotation=45, ha='right')  # X축 눈금 이름
    ax.legend()  # 범례 추가

    plt.ylim(0, max(max(success_values), max(failure_values)) + 1)  # Y축 범위 설정
    plt.grid(axis='y', linestyle='--', alpha=0.7)  # 격자 표시

    # 막대 위에 값 표시
    for i in range(len(categories)):
        ax.text(i, success_values[i] + 0.2, str(success_values[i]), ha='center', va='bottom', color='black')
        ax.text(i + bar_width, failure_values[i] + 0.2, str(failure_values[i]), ha='center', va='bottom', color='black')

    plt.tight_layout()

    # 그래프를 파일로 저장
    os.makedirs(os.path.dirname(output_path), exist_ok=True)  # 디렉토리 생성
    plt.savefig(output_path)
    print(f"Graph saved to {output_path}")
    plt.close()


# 파일에서 결과 읽기
test_file_path = 'test_all.txt'  # 테스트 파일 경로
results = read_test_output_from_file(test_file_path)

if results:
    output_file = "../graph/success_rate_plot.png"  # 저장될 그래프 파일 경로
    test_cases = 1000  # 테스트 케이스 개수
    bit_length = 4096  # 비트 길이
    plot_success_rate(results, output_file, test_cases, bit_length)
