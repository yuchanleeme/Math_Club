from ctypes import cdll
import ctypes

MathCalculator = cdll.LoadLibrary('MathCalculator.dll')

res = "15G9"
print(MathCalculator.calculator(res))
