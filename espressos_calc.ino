/*
			THIS IS MADE BY FEUSX IN 21-09-2025
*/

#include <Arduino.h>
#include <stdlib.h>

constexpr size_t INPUT_BUFFER_SIZE = 64;
char input_buffer[INPUT_BUFFER_SIZE];

double result = 0.0;
bool first_input = true;

bool isNumeric(const char *str);
void trim(char *str);

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("EspressOS Calculator");
  Serial.print("Enter initial number: ");
}

void loop()
{
  size_t len = Serial.readBytesUntil('\n', input_buffer, INPUT_BUFFER_SIZE - 1);
  if(len == 0) return;

  input_buffer[len] = '\0';
  trim(input_buffer);
  if(input_buffer[0] == '\0') return;

  if(strcasecmp(input_buffer, "q") == 0)
  {
    Serial.println("Calculator exited.");
    while(true);
  }

  if(first_input)
  {
    if(!isNumeric(input_buffer))
    {
      Serial.println("Invalid number.");
      Serial.print("Enter initial number: ");
      return;
    }

    result = strtod(input_buffer, nullptr);
    first_input = false;
  }
  else
  {
    char op = input_buffer[0];
    char *operand_str = input_buffer + 1;
    trim(operand_str);

    if((op == '+' || op == '-' || op == '*' || op == '/') && isNumeric(operand_str))
    {
      double operand = strtod(operand_str, nullptr);

      switch(op)
      {
        case '+':
          result += operand;
          break;
        case '-':
          result -= operand;
          break;
        case '*':
          result *= operand;
          break;
        case '/':
          if(operand == 0.0)
          {
            Serial.println("Error: Division by zero.");
            return;
          }
          result /= operand;
          break;
      }
    }
    else if(isNumeric(input_buffer))
    {
      result = strtod(input_buffer, nullptr);
    }
    else
    {
      Serial.println("Syntax error. Use: [+|-|*|/]number or just a number.");
      return;
    }
  }

  Serial.print("= ");
  Serial.println(result, 6);
  Serial.print("> ");
}

bool isNumeric(const char *str)
{
  if(!str || *str == '\0') return false;
  char *endptr;
  strtod(str, &endptr);
  return (*endptr == '\0');
}

void trim(char *str)
{
  if(!str) return;

  char *start = str;

  while(isspace((unsigned char)*start)) start++;
  if(*start == '\0') { *str = '\0'; return; }

  char *end = start + strlen(start) - 1;

  while(end > start && isspace((unsigned char)*end))
  {
    *end = '\0';
    end--;
  }

  if(start != str) memmove(str, start, strlen(start) + 1);
}
