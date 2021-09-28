---
title: "Contribute to this site"
layout: base
name: contribute
---

# Formatting
We aim to provide a uniform look and feel across the site.

Blockquotes are used to put emphasis on quoted text. The standard
way of doing it in Markdown is to prepend the lines that need this
formatting with the '>' character. For example:
```
>Blockquote!
```
...will produce
>Blockquote!

Language-aware code highlighting capability has been integrated and
can be easily used via the fenced code blocks i.e. blocks delimited by
triple backticks, with an optional shortcut for the programming language name.
For example,



    ```bash
    export foo="foo"
    echo $foo
    ls -l .
    ```

will be rendered as:
```bash
export foo="foo"
echo $foo
ls -l .
```

and

    ```python
    def my_function():
      print("Hello from a function")
    ```

will show as:

```python
def my_function():
  print("Hello from a function")
```
For C++, the opening line should be triple backticks followed by "c++"
similar to examples above and the code will be rendered accordingly such as:

```c++
class MyClass {       // The class
  public:             // Access specifier
    int myNum;        // Attribute (int variable)
    string myString;  // Attribute (string variable)
};
```