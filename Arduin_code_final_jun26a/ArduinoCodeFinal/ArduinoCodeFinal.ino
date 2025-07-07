String input = "";

void setup() {
  // put your setup code here, to run once:
  // Begin serial communication baud rate 9600
  Serial.begin(9600); 
  // Wait until the serial monitor is open and ready
  while (!Serial); 
}

void loop() {
  // Checks if there is any data available fromt the serial port
  if (Serial.available()) { 
    // Reads the input string until a newline character \n
    input = Serial.readStringUntil('\n'); 
    // Removes leading and traiing whitespace or newline characters
    input.trim(); 
    // Remove carriage return
    input.replace("\r", "");

    String num1Str;
    String arithOperatorStr;
    String num2Str;


    // Finds the first and last space to split the input
    int space1 = input.indexOf(' ');
    int space2 = input.lastIndexOf(' ');

    if (space1 > 0 && space2 > space1) {
      // Extract the substrings format "5 + 3"
      num1Str = input.substring(0, space1);
      arithOperatorStr = input.substring(space1 + 1, space2);
      num2Str = input.substring(space2 + 1);
    }
    else {
      // Format without spaces: "5+3"
      int arithOperatorPos = -1;
      // Find the operator position (start from index 1 to handle negative number)
      for (int i = 1; i < input.length(); i++) {
        char c = input.charAt(i);
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' || c == 'r') {
          arithOperatorPos = i;
          break;
        }
      }
      if (arithOperatorPos > 0) {
        num1Str = input.substring(0, arithOperatorPos);
        arithOperatorStr = input.substring(arithOperatorPos, arithOperatorPos + 1);
        num2Str = input.substring(arithOperatorPos + 1);
      }
      else {
        Serial.println("Error: No valid operator found");
        return; // Exit early if no operator found
      }
    }

    // Check if we successfully parsed the input
    if (num1Str.length() > 0 && arithOperatorStr.length() > 0 && num2Str.length() > 0) {
      // Variable to store the input number and operator and convert strings to float
      float num1 = num1Str.toFloat();
      float num2 = num2Str.toFloat();
      char arithOperator = arithOperatorStr.charAt(0);

      // Variable to hold result and check if operation is valid
      float result;
      bool valid = true;

      // Performs operation based on the operator inputted
      switch (arithOperator) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
          if (num2 != 0) result = num1 / num2;
          else {
          Serial.println("Error: Division by zero");
          valid = false;
          }
          break;
        case '^': 
          // Checks if the power is of long type or decimal
          if (num2 == (long)num2) result = round(pow(num1, num2));
          else {
            result = pow(num1, num2);
          }
          break;
        case 'r': 
          //float tempResult;
          //tempResult = pow(num2, (1.0/num1));
          result = pow(num2, (1.0/num1)); 
          // Checks if result is effectively a whole number
          if (fabs(result - round(result)) < 0.0001) {
            result = round(result);
          }
          break;
        case '%': result = (int)num1 % (int)num2; break;
        default:
          Serial.println("Error: Invalid operator"); //Handles invalid operators
          valid = false;
        }

      // Print result
      if (valid) {
        //Serial.print("Result: ");
        // Print without decimal if result is a whole number
        if (result == (long)result) { 
          Serial.print(num1Str + " " + arithOperatorStr + " " + num2Str + " = ");
          Serial.println(round(result));
        }
        else {
          Serial.print(num1Str + " " + arithOperatorStr + " " + num2Str + " = ");
          Serial.println(result, 4); // Print with 6 decimal place
        }
      }
    }
    else {
      // If input format was incorrect
      Serial.println("Error: Invalid input format");
    }
  }
}