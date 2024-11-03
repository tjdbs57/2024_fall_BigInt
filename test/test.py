import matplotlib.pyplot as plt
import time

def evaluate_expression(expression):
    parts = expression.replace(' ', '').split('==')
    if len(parts) != 2:
        return False

    left_side = parts[0]
    right_side = parts[1]

    # Evaluate 
    try:
        left_value = eval(left_side)
        right_value = eval(right_side)
        return left_value == right_value
    except Exception as e:
        print(f"Error evaluating expression '{expression}': {e}")
        return False

def read_success_rate_from_file(file_path):
    try:
        # Open the file with UTF-8 encoding
        with open(file_path, 'r', encoding='utf-8') as file:
            lines = file.readlines()
        
        success_count = 0
        total_count = len(lines)
        
        for line in lines:
            stripped_line = line.strip()
            if evaluate_expression(stripped_line):
                success_count += 1

        success_rate = (success_count / total_count) * 100 if total_count > 0 else 0
        return success_rate, success_count, total_count
    except FileNotFoundError:
        print(f"No such file: {file_path}")
        return 0, 0, 0  
    except Exception as e:
        print(f"Error reading the file: {e}")
        return 0, 0, 0  

def plot_success_rate(success_rate, success_count, total_count):
    plt.figure(figsize=(8, 6))
    categories = ['Success', 'Failure']
    values = [success_count, total_count - success_count]
    
    plt.bar(categories, values, color=['green', 'red'])
    plt.ylabel('Count')
    plt.title(f'Success Rate: {success_rate:.2f}% ({success_count}/{total_count})')
    plt.ylim(0, total_count)  
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    
    # Add the percentage on top of the bars
    for index, value in enumerate(values):
        plt.text(index, value, f'{value}', ha='center', va='bottom')

    plt.show()


file_path = 'test.txt' 
 
# start time
start_time = time.time() 
success_rate, success_count, total_count = read_success_rate_from_file(file_path)
end_time = time.time()
execution_time = end_time - start_time

with open("log.txt", "a") as log_file:
    log_file.write(f"verifying took {execution_time: f} seconds\n")

if total_count > 0:
    plot_success_rate(success_rate, success_count, total_count)