# LINQpp

LINQ implementation in C++

## Repo status

![Build](https://github.com/qjcina/LINQpp/workflows/Build/badge.svg) ![badge](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/qjcina/beb364d3e95c4db5ef641cd01ff61634/raw/test.json)

## About

LINQ is .NET platform component which simplifies data processing by introducing set of query-like expressions. LINQpp is an attempt to bring this feature into C++ world.

## Usage

### Project

LINQpp is header-only library. All you have to do to use it is to include `src/LINQpp` folder in your project.
Project in this repository contains 2 subdirectories: example (`src/main.cpp`) and unit tests. You can compile and check them out as well for better understanding of the project.

### LINQpp philosophy

LINQpp under the hood tries to aviod copying your data. Library provides 2 entry points: `linq::from` and `linq::copyFrom`. First one holds reference to provided container, any operation that can cause iterator invalidation is forbidden and can cause unexpected behaviour. Second one holds copy of your whole container in the root of all operations, so if you want to avoid copying whole container you should use `linq::from`.  
At any point you can evaluate whole expression to create new root or output container by assigning it to valid container with assignment operator or by calling `forceEvaluate`.

### Query Operators

LINQpp is still in progres, so not all of LINQ query operators are available.

❌ Select  
✔️ Where - filters out not matching elements

```c++
linq::from(array)->where("element");                                              // compare with literal value
linq::from(array)->where([](const auto& element){ return element.size() > 2; });  // compare with lambda expression
```

❌ SelectMany  
❌ Sum / Min / Max / Average  
❌ Aggregate  
❌ Join / GroupJoin  
❌ Take / TakeWhile  
❌ Skip / SkipWhile  
❌ OfType  
❌ Concat  
❌ OrderBy / ThenBy  
❌ Reverse  
❌ GroupBy  
❌ Distinct  
❌ Union / Intersect / Except  
❌ SequenceEqual  
✔️ First / FirstOrDefault / Last / LastOrDefault

```c++
// first / last throw std::logic_error when element is not found
linq::from(array)->first("element");
linq::from(array)->first([](const std::string& element)
                        { return element.size() > 2; });
// firstOrDefault / lastOrDefault are available only for class container types that
// are default constructible or basic types
linq::from(array)->firstOrDefault("element");
linq::from(array)->firstOrDefault([](const std::string& element)
                                 { return element.size() > 2; });
```

❌ Single  
❌ ElementAt  
✔️ Any / All

```c++
// any checks for single occurence in container
linq::from(array)->any("element");
linq::from(array)->any([](const auto& element){ return element.size() > 2; });
// all checks if all elements match predicate or literal
linq::from(array)->all("element");
linq::from(array)->all([](const auto& element){ return element.size() > 2; });
```

✔️ Contains  
Alias to `any`.

❌ Count
