class BigNumberCalculator:
    def add(self, num1: str, num2: str) -> str:
        result = int(num1, 16) + int(num2, 16)
        return hex(result)

    def subtract(self, num1: str, num2: str) -> str:
        result = int(num1, 16) - int(num2, 16)
        return hex(result)

    def multiply(self, num1: str, num2: str) -> str:
        result = int(num1, 16) * int(num2, 16)
        return hex(result)

    def divide(self, num1: str, num2: str) -> str:
        try:
            result = int(num1, 16) // int(num2, 16)
            return hex(result)
        except ZeroDivisionError:
            return "Error: Division by zero is not allowed."

# Calculator instance
calculator = BigNumberCalculator()

# User input for numbers and operation
num1 = input("Enter the first large hexadecimal number: ")
num2 = input("Enter the second large hexadecimal number: ")
print("Choose an operation:")
print("1: Addition (+)")
print("2: Subtraction (-)")
print("3: Multiplication (*)")
print("4: Division (/)")
operation = input("Enter the number of the operation (1, 2, 3, or 4): ")

# Perform chosen operation
if operation == "1":
    result = calculator.add(num1, num2)
elif operation == "2":
    result = calculator.subtract(num1, num2)
elif operation == "3":
    result = calculator.multiply(num1, num2)
elif operation == "4":
    result = calculator.divide(num1, num2)
else:
    result = "Error: Invalid operation."

print("Result:", result)