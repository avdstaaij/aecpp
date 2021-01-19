# AEC++ [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Yet another small C++11 library for colors and effects in terminal ouput by
means of <ins>a</ins>nsi <ins>e</ins>scape <ins>c</ins>odes



## Quick example

```cpp
#include <iostream>
#include "aecpp.h"

int main() {
	std::cout << "Normal text\n"
	          << aec::red << "Red text!\n" << aec::reset
	          << aec::bold + aec::blue << "Bold blue text!!\n" << aec::reset;

	auto style = aec::green + aec::italic;

	std::cout << style << "Green italic text!!!\n" << aec::reset;

	return 0;
}
```


## Installation

AEC++ is a single-file header-only library, consisting only of the file
[`aecpp.h`](include/aecpp.h) in the `include` directory. As long as you adhere
to the [license](LICENSE.txt), this file is all you need. Simply copy it to a
place where your compiler can find it, or use a compiler flag to add the
`include` directory to your compiler's include directory list.



## How to use

### Basic usage

AEC++ has a simple interface, based on C++ stream objects such as `cout`.
To enable a color or an effect, output the appropriate value (e.g. `aec::red`)
to the stream, as shown in the example above. To turn it off again, output
`aec::reset`. The values can be divided into four categories: *text colors*,
*background colors*, *text effects* and *resetting values*. The complete list of
outputtable values is shown below:

**Text colors**
| Value                  | Effect                    |
| :--------------------- | :------------------------ |
| `aec::black`           | Black text                |
| `aec::red`             | Red text                  |
| `aec::green`           | Green text                |
| `aec::yellow`          | Yellow text               |
| `aec::blue`            | Blue text                 |
| `aec::magenta`         | Magenta text              |
| `aec::cyan`            | Cyan text                 |
| `aec::gray`            | Gray text                 |
| `aec::brightblack`     | Bright black text         |
| `aec::brightred`       | Bright red text           |
| `aec::brightgreen`     | Bright green text         |
| `aec::brightyellow`    | Bright yellow text        |
| `aec::brightblue`      | Bright blue text          |
| `aec::brightmagenta`   | Bright magenta text       |
| `aec::brightcyan`      | Bright cyan text          |
| `aec::brightgray`      | Bright gray text          |

**Background colors**
| Value                  | Effect                    |
| :--------------------- | :------------------------ |
| `aec::bgblack`         | Black background          |
| `aec::bgred`           | Red background            |
| `aec::bggreen`         | Green background          |
| `aec::bgyellow`        | Yellow background         |
| `aec::bgblue`          | Blue background           |
| `aec::bgmagenta`       | Magenta background        |
| `aec::bgcyan`          | Cyan background           |
| `aec::bggray`          | Gray background           |
| `aec::bgbrightblack`   | Bright black background   |
| `aec::bgbrightred`     | Bright red background     |
| `aec::bgbrightgreen`   | Bright green background   |
| `aec::bgbrightyellow`  | Bright yellow background  |
| `aec::bgbrightblue`    | Bright blue background    |
| `aec::bgbrightmagenta` | Bright magenta background |
| `aec::bgbrightcyan`    | Bright cyan background    |
| `aec::bgbrightgray`    | Bright gray background    |

**Text effects**
| Value                  | Effect                    |
| :--------------------- | :------------------------ |
| `aec::bold`            | Bold text                 |
| `aec::dim`             | Dim text                  |
| `aec::italic`          | Italic text               |
| `aec::underline`       | Underlined text           |
| `aec::blink`           | Blinking text             |
| `aec::rapidblink`      | Rapidly blinking text     |
| `aec::reversevideo`    | Inverted colors           |
| `aec::conceal`         | Invisible text            |
| `aec::crossout`        | Strikethrough text        |

**Resetting values**
| Value                  | Effect                    |
| :--------------------- | :------------------------ |
| `aec::creset`          | Reset text color          |
| `aec::bgcreset`        | Reset background color    |
| `aec::reset`           | Reset all styling         |

Note that text effects cannot be reset separately.

Under the hood, outputting these values to a stream object causes special ansi
escape codes to be printed. If these codes are printed to a terminal that
supports them (i.e. any modern terminal), then they are invisible and trigger
the described effects. However, if the codes are printed to a file or a
terminal that doesn't support them, the raw codes will be visible. To address
this problem, AEC++ by default only actually outputs the codes if the output
stream is connected to a terminal that supports them. This means that you don't
need to worry about the environment in which your program is used or whether
users will redirect your program's output to a file.

Note:\
When using background colors, it is important to use aec::reset before writing
a newline. Otherwise, the next line may be affected.

### Style objects

In addition to the ansi escape code values, AEC++ provides the `aec::Style`
class. This class represents a combination of escape code values that together
form a style. Using `Style` objects, such styles can be stored and changed via
variables. Styles can be constructed by adding code values with the `+`-operator
(as is done in the quick example at the top). They can also be constructed
explicitly. This is shown in the [additional examples](#Additional-examples).
Style objects can also be modified using its public methods `setColor`,
`unsetColor`, `setBGColor`, `unsetBGColor`, `setEffect`, `unsetEffect` and
`setReset`.

A minor technical detail: when outputting styles, its three types of escape
codes (text color, background color and text effects) are combined into a single
code. This saves a few characters.

### Output modes

As stated, the escape codes are by default only actually printed if the output
stream is connected to a terminal that supports them. You may however not always
want this behavior. AEC++ provides four different output modes. The active
output mode can be changed at runtime using `aec::setMode`. This function takes
an `aec::Mode` enum value. The available modes are:

- `aec::Mode::Always`\
  Always print escape codes.

- `aec::Mode::Auto` (default)\
  Print escape codes if and only if the output stream is connected to a terminal
  that supports them.

- `aec::Mode::Semi`\
  Print escape codes if and only if the program is running in an environment
  that supports them, but do not check if the output stream is connected to a
  terminal. This may be useful if an AEC++ program is used in a pipeline.

- `aec::Mode::Never`\
  Never print escape codes.

Terminal support of ansi escape codes is currently determined solely based on
the environment variable `$TERM`. View `aecpp.h` for the values of `$TERM` that
AEC++ considers ansi-escape-code-supporting.

To improve performance, AEC++ does not check whether the output stream is
connected to a terminal for every printed escape code. Instead, it checks this
only once, at the start of the program. This is not a problem in the vast
majority of use cases. However, if you do somehow change the targets of the
standard output streams at runtime, you can call
`aec::checkIfOutputStreamsAreTerminals` to make AEC++ re-check them. The length
of that function name should serve as an indication of how unlikely it is that
you need it :).

## Additional examples

Additional example code can be found in
[`examples/examples.cpp`](examples/examples.cpp). These examples can also be
compiled and executed by using `make run` in the `examples` directory.



## Attributions

AEC++ was inspired by rang: https://github.com/agauniyal/rang
