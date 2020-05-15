from ctypes import cdll
import ctypes

MathCalculator = cdll.LoadLibrary('MathCalculator.dll')

res = "12r150"
print(MathCalculator.calculator(res))




