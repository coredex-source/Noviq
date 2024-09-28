import re
import sys

def display(format_string, *variables):
    for i in range(len(variables)):
        format_string = format_string.replace(f"(%var{i + 1})", str(variables[i]))
    print(format_string)

def evaluate_expression(expression, variables):
    try:
        for var in variables:
            expression = expression.replace(var, str(variables[var]))
        result = eval(expression)
        return result
    except Exception as e:
        raise SyntaxError(f"Invalid expression: {expression}")

def interpretor(line, variables, declared_vars, line_number):
    stripped_line = line.strip()
    if not stripped_line:
        return

    # Variable declaration: let x be an Integer/String/Float
    declaration_match = re.match(r"let\s+(\w+)\s+be\s+(a|an)\s+(String|Integer|Float)", stripped_line)
    if declaration_match:
        var_name = declaration_match.group(1)
        var_type = declaration_match.group(3)
        if var_type == "String":
            variables[var_name] = ""
        elif var_type == "Integer":
            variables[var_name] = 0
        elif var_type == "Float":
            variables[var_name] = 0.0
        declared_vars[var_name] = True
        return

    # Assignment with let: let x be 5
    assignment_match = re.match(r"let\s+(\w+)\s+be\s+(.+)", stripped_line)
    if assignment_match:
        var_name, value = assignment_match.groups()
        try:
            variables[var_name] = eval(value)
        except Exception as e:
            variables[var_name] = value
        declared_vars[var_name] = True
        return

    # Check if the variable was declared before performing assignment or arithmetic
    assignment_match = re.match(r"(\w+)\s*=\s*(.+)", stripped_line)
    if assignment_match:
        var_name, expression = assignment_match.groups()
        if var_name not in declared_vars:
            raise SyntaxError(f"Syntax Error: Variable '{var_name}' not declared before assignment, caused at line {line_number}")
        try:
            variables[var_name] = evaluate_expression(expression, variables)
        except SyntaxError as e:
            raise SyntaxError(f"Syntax Error: {line.strip()}, caused at line {line_number}")
        return

    # Display statement: display("Text (%var1)...", x)
    print_syntax_check = re.match(r"display\s*\(\s*\"([^\"]*)\"\s*(,.*)?\)", stripped_line)
    if print_syntax_check:
        format_string = print_syntax_check.group(1)
        vars_to_display = []
        if print_syntax_check.group(2):
            var_names = re.findall(r"(\w+)", print_syntax_check.group(2))
            for var_name in var_names:
                if var_name in variables:
                    vars_to_display.append(variables[var_name])
                else:
                    vars_to_display.append("undefined")
        display(format_string, *vars_to_display)
        return

    raise SyntaxError(f"Syntax Error: {line.strip()}, caused at line {line_number}")

def litecode(filename):
    try:
        with open(filename, "r") as file:
            code = file.readlines()
        variables = {}
        declared_vars = {}
        for line_number, line in enumerate(code, start=1):
            try:
                interpretor(line, variables, declared_vars, line_number)
            except SyntaxError as e:
                print(e)
                sys.exit(1)
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: litecode <filename>")
    else:
        litecode(sys.argv[1])
