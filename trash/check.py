def check_expression():

    expression = input("Enter the expression to check : ").strip()

    # '==' 기준으로 나누기
    try:
        left_expr, right_expr = expression.split("==")
    except ValueError:
        print("Invalid expression format. Ensure the expression contains '=='.")
        return

    # 양쪽 계산
    try:
        left_result = eval(left_expr.strip())
        right_result = eval(right_expr.strip())
    except Exception as e:
        print(f"Error evaluating expression: {e}")
        return

    # 결과 비교
    if left_result == right_result:
        print("The result is correct!")
    else:
        print("The result is incorrect.")
        print(f"Left result:  {hex(left_result) if isinstance(left_result, int) else left_result}")
        print(f"Right result: {hex(right_result) if isinstance(right_result, int) else right_result}")

# 실행
check_expression()
