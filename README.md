soruceAFISC++ folder contains build.py file and cmake file and jni folder.

To build library go to terminal and run command: python build.py

jni folder contains 2 subfolders jni_common and jni_detection both folders contains same files but jni_common folder to build object files (.o) and jni_detection folder to build shared object files (.so) which is used in java as library.

To test the code on DevC++ follow given steps to setup DevC++ project configuration.
1- Goto project options -> Compiler move to Code Generation tab and set Optimization level (-Ox) to "Med" and Language standard (-std) to "ISO C++11".

2- Goto project options -> Parameters set C++ compiler param "-lgdi32".

3- To run the code run main.cpp file.