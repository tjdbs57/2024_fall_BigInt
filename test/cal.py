class BigNumberCalculator:
    """
    @brief A calculator for performing arithmetic operations on large hexadecimal numbers.
    @details This class provides methods to perform addition, subtraction, multiplication, 
             division, and modular arithmetic on hexadecimal numbers represented as strings.
    """

    def add(self, num1: str, num2: str) -> str:
        """
        @brief Adds two large hexadecimal numbers.
        @param num1 A string representing the first hexadecimal number.
        @param num2 A string representing the second hexadecimal number.
        @return A string representing the result of the addition in hexadecimal.
        """
        result = int(num1, 16) + int(num2, 16)
        return hex(result)

    def subtract(self, num1: str, num2: str) -> str:
        """
        @brief Subtracts the second hexadecimal number from the first.
        @param num1 A string representing the first hexadecimal number.
        @param num2 A string representing the second hexadecimal number.
        @return A string representing the result of the subtraction in hexadecimal.
        """
        result = int(num1, 16) - int(num2, 16)
        return hex(result)

    def multiply(self, num1: str, num2: str) -> str:
        """
        @brief Multiplies two large hexadecimal numbers.
        @param num1 A string representing the first hexadecimal number.
        @param num2 A string representing the second hexadecimal number.
        @return A string representing the result of the multiplication in hexadecimal.
        """
        result = int(num1, 16) * int(num2, 16)
        return hex(result)

    def divide(self, num1: str, num2: str) -> str:
        """
        @brief Divides the first hexadecimal number by the second.
        @param num1 A string representing the numerator (first hexadecimal number).
        @param num2 A string representing the denominator (second hexadecimal number).
        @return A string representing the result of the division in hexadecimal.
        @exception ZeroDivisionError If the second number is zero, returns an error message.
        """
        try:
            result = int(num1, 16) // int(num2, 16)
            return hex(result)
        except ZeroDivisionError:
            return "Error: Division by zero is not allowed."

    def modular(self, num1: str, num2: str) -> str:
        """
        @brief Computes the remainder of the division of two hexadecimal numbers.
        @param num1 A string representing the first hexadecimal number.
        @param num2 A string representing the second hexadecimal number.
        @return A string representing the result of the modular operation in hexadecimal.
        @exception ZeroDivisionError If the second number is zero, returns an error message.
        """
        try:
            result = int(num1, 16) % int(num2, 16)
            return hex(result)
        except ZeroDivisionError:
            return "Error: Division by zero is not allowed."


# Calculator instance
calculator = BigNumberCalculator()

# User input for numbers and operation
num1 = input("Enter the first large hexadecimal number: ")  # Example: 0x1A3
num2 = input("Enter the second large hexadecimal number: ")  # Example: 0x4F
print("Choose an operation:")
print("1: Addition (+)")
print("2: Subtraction (-)")
print("3: Multiplication (*)")
print("4: Division (/)")
print("5: Modular (%)")
operation = input("Enter the number of the operation (1, 2, 3, 4, or 5): ")

# Perform chosen operation
if operation == "1":
    result = calculator.add(num1, num2)
elif operation == "2":
    result = calculator.subtract(num1, num2)
elif operation == "3":
    result = calculator.multiply(num1, num2)
elif operation == "4":
    result = calculator.divide(num1, num2)
elif operation == "5":
    result = calculator.modular(num1, num2)
else:
    result = "Error: Invalid operation."

# Display the result
print("Result:", result)
