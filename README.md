# Dezibot4 Lib

## Links to external documentation
* [PDF-Doku Code](https://hardwarelabor.imn.htwk-leipzig.de/dezibot/dezibot.pdf)
* [PDF-Doku Device](https://hardwarelabor.imn.htwk-leipzig.de/dezibot/dezibot-4doku.pdf)

## Link to Software
* [Library](https://github.com/dezibot/dezibot)

## Introduction

The project focuses on the software for the Dezibot4 robot and its use in the classroom.<br>
It includes libraries for use by students as well as guides for teachers. The hardware of the robot is not part of the project.<br>
The libraries and example programs are available under the GPL and are accessible as a repository.<br>
It is ment to serve as an Arduino-Library.<br> 
Therefore the rules for arduinolibrary develop apply:<br>

* [Styleguide](https://docs.arduino.cc/learn/contributions/arduino-library-style-guide)
* [Libraryspecification](https://arduino.github.io/arduino-cli/0.35/library-specification/)
* [Submission-requirements](https://github.com/arduino/library-registry/blob/main/FAQ.md#submission-requirements)

In the following the most important points and custom conventions are introduced.

## Code Conventions

### Don't pass reference

To allow easy usability for users not familier with C++, prevent passing around references. It is better to use
accessmethods

### Naming

* methods are named in lowerCamelCase
* classes are named in UpperCamelCase
* folders containing components are named in lowerCamelCase
* methods are named in lowerCamelCase
* constants are named in ALL_CAPS_SNAKE_CASE

### Bytestream

Every class that implements Byte-Based Communication needs to implement the Arduino Streaminterface

### Components

Every component has a single .h file and one or more .cpp files.<br>
Every component is placed in a seperate folder under src/ that is named equvivalent to the class.
The minimal structure of any .h file is<br>

```c++
#ifndef ClassName_h
#define ClassName_h
class ClassName{

};
#endif //ClassName_h
```

## Design Paradigm

During desgin, the Dezibot isn't describe using it's part but instead it's functionality. Under the top-level
Dezibotclass, there is a class for every functionality of the robot. Each of that classes consists of two parts.

### Part Instances

Each component contains instances of every Robotpart that is used in that component. For example the Motion component
contains two motorinstances, one for motorEast and one for motorWest.
Using these instances, it is possible to access more specific methods that interacts directly with the component (
configure it, setSpeed,...)

### Abstractions

The components constains abstractions that combines multiple partMethods to ease the usability. For example for the
motioncomponent provides an abstraction for the forwardmovement, that involves two motors and even another component (
MotionDetection)

## Contributing

When contributing to the project please follow the rules below. At first, follow all rules from this readme. Further
rules apply to the usage of git

### Branching

Whenever working on the project, create a new branch from the current state of Develop.
Branches should be named as `prefix/#issueid-shortdescription` where prefix is from {feature,fix,refactor}.<br>
When a branch is ready to be used in production, create a mergerequest.

### Mergerequests

The target of each Mergerequest must be the Develop-Branch. Before the merge, each request must be approved by at least
one person with Owner role.<br>
The approve process should consider especially the documentation, naming, implementation.
When the merge is approved and no more commits are added, the last commit must increment the versionnumber in the
library.properties file, following the rules of [Semantic Versioning](https://semver.org/)

### Commitmessages

Commitmessages must follow
the [gitchangelog](https://github.com/vaab/gitchangelog/blob/master/src/gitchangelog/gitchangelog.rc.reference) pattern.

### Language

The language of the project is American English. That includes in particular but not exclusively:

* Sourcecode
* Commit Messages
* Documentation

A german documentation will be provided but does not replace the english documentation.

### Documentation

Documentation of the Software and Hardware can be found at https://docs.dezibot.de/

#### .h Files

```C++
/**
 * @file Dezibot.h
 * @author your name (you@domain.com)
 * @brief 
 * @date 2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

```

In the library, the `.h` files should be included using a relative path.
For instance, in `src/Dezibot.h`, to include `src/motion/Motion.h`, you should write `#include "motion/Motion.h"`.

#### Methods

```C++
/**
 * @brief
 * @param
 * ...
 * @return
 *
 */
```

#### Arduino Settings

* Board: "ESP32-S3-USB-OTG"
* Upload Mode: "UART0 / Hardware CDC"
* USB Mode: "Hardware CDC and JTAG"
* Programmer: "Esptool"

Using `arduino-cli` to compile and upload:
`arduino-cli upload /Users/jo/Documents/Arduino/theSketch -p /dev/cu.usbmodem101 -b esp32:esp32:nora_w10`
`arduino-cli compile /Users/jo/Documents/Arduino/theSketch -p /dev/cu.usbmodem101 -b esp32:esp32:nora_w10`

##### Including Library

Arduino IDE -> Sketch -> Include Library -> add .ZIP Library -> this library

If there is any other error like 'Painless_Mesh' not found, you have to include this library also. 

Arduino IDE -> Sketch -> Manage Library -> Search for missing Library

#### Start from Scratch

It is important, before using any functions of Dezibot, to call ```dezibot.begin()``` once in the setup function.

In the examples folder, a sketch ``start`` is provided, that handles the initialization.
