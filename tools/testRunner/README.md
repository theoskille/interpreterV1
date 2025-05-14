# Lox Interpreter Test Runner

This simple tool allows you to test various components of your Lox interpreter implementation without having to modify your main application code.

## Features

1. **Scanner Testing**: Test the scanner by providing source code and view the tokens it produces.
2. **Parser Testing**: Test the parser by entering expressions and view the resulting AST.
3. **Manual AST Testing**: Try out a pre-built AST to verify your AST printer.

## Usage

### Building

```bash
# Navigate to the test runner directory
cd tools/testRunner

# Build the test runner
make

# Run the test runner
make run
# or directly
./test_runner
```

### Modifying Tests

The primary purpose of this tool is to allow you to quickly test components as you build them. You can (and should) modify the test functions in `TestRunner.cpp` as needed:

- To test new language features
- To focus on specific areas of functionality
- To create regression tests for bugs you fix

## Example Tests

### Scanner Test
Enter source code like `var x = 10;` to see how it gets tokenized.

### Parser Test
Enter expressions like `1 + 2 * 3` to see the resulting AST.

### Manual AST Test
The default test creates an AST for `(123 + 456) * 789`. Modify this to test more complex expressions or specific edge cases.

## Customization

Feel free to modify this test runner frequently as you develop your interpreter. It's designed to be a flexible test bed for you to experiment with and validate your code. 