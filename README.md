# Expression Parser



### Description
Expression Parser is an algebraic expression parser. The parser takes an input string expression and an input map of terms to parse & construct an expression tree for evaluation. The expression is parsed with recursive `std::regex` evaluations and the expression tree is constructed with `std::shared_ptr` polymorphic nodes that represent various mathematical objects (constants, variables, matrices, tensors, operations, functions, etc.)



### Building
Built on Ubuntu 20.04.1 LTS compiled with GCC 9.3.0.

To build & run the unit test:
```
cd test
mkdir build && cd build
cmake .. && make
./ExpressionParserTest
```

To build & run the provided examples:
```
mkdir build && cd build
cmake .. && make
cd Examples/MatrixExample && ./MatrixExample
cd Examples/WavefunctionExample && ./WavefunctionExample
```
