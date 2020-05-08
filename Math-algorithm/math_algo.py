from ctypes import cdll
import ctypes

MathCalculator = cdll.LoadLibrary('MathCalculator.dll')

res = "3P3"
print(MathCalculator.calculator(res))




