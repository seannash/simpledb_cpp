# SimpleDB In C++

This repo is based on the book Database Design and Implementation (Second Edition) by Edward Sciore. The book is in Java and this repo has it reimplemented in C++.

Some of the code is not in the book such as SimpleIJ, as the authors website is currently down, the reference for that was taken from https://github.com/eatonphil/sciore-simpledb-3.4 which has the authors
code uploaded to Github.

# Todo

## Complete the book

Not done yet.

## Unit testing

Add unit tests using catch2. Testing will be an exploration of how the system works.

## Pointers/Referneces

Java uses references for all objects, so passing them around is cheap. Translated this as a std::share_ptr. Need to analyze what class owns what to determien if a pointer (convention bein not owning) could simplify the code.

## String view

The class std::string_view can allow for eaise of use of methods (taking in std::string, raw c string, or another std::string_view), but seems unnessisary here. Changing the functions that are internal to the the database to
"const std::string&" would be less object creation.

## Naming Convention

Switched between camel casing and snake casing. Will switch all code to camel casing to closer match the book for ease of comparison. Some differences will remain such as delete being a C++ keyword cannot be a function name.