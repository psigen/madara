INTRODUCTION

This folder contains the Python port for MADARA. These instructions will
explain how to install and use the Python port.

AUTHOR(s)

James Edmondson <jedmondson@gmail.com>

PREREQUISITES

1.) Python

  1.a) Obtain python and any dev libraries (Linux users require this step)
  1.b) Set environment variables
    
       PYTHON_ROOT    The location of your Python install folder
       PYTHON_VERSION The major and minor version number (e.g. 33 for 3.3)
    
       PYTHON_VERSION is used to locate the python{PYTHON_VERSION).lib

2.) Boost.Python

  2.a) Set environment variables
    
       BOOST_ROOT      The location of your Boost install folder
       BOOST_VERSION   The version number (e.g. 1_55)
         
       BOOST_VERSION is used to locate the boost.python library
    
  2.b) Setup your user-config.jam file in your home directory
    
       Example on Windows in C:\Users\<username>, using VS 2010
       and Python version 3.3 (32 bit)
         
         using msvc : 10.0 ;
         using python
           : 3.3
           : C:/Python33/32bit
         ;
         
  2.c) Compile the Boost.Python Library
 
       1. cd to BOOST_ROOT
       2. Run bootstrap in the local directory
       3. Run "b2 link=shared threading=multi stage"
         
3.) Enable the python feature in your MADARA install

    LINUX:
    
      cd $MADARA_ROOT
      mwc.pl -type gnuace -features python=1 MADARA.mwc
      make tests=1 python=1
      
    WINDOWS:
    
      cd %MADARA_ROOT%
      mwc.pl -type vc10 -features python=1 MADARA.mwc
      *open Visual Studio solution*
 
4.) Integrate the generated MADARA_Python.dll into your Python environment
