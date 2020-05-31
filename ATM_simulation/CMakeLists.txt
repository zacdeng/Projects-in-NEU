cmake_minimum_required(VERSION 3.15)
project(Bank_simulate)

set(CMAKE_CXX_STANDARD 14)

add_executable(Bank_simulate main.cpp UI_Design.cpp UI_Design.h BankAcc.cpp BankAcc.h)