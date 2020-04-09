from ctypes import cdll
import ctypes

MathCalculator = cdll.LoadLibrary('MathCalculator.dll')

res = "20-5*(7-3)/2"
print(MathCalculator.calculator(res))
















def convert_ctype(x):
    return x.encode('utf-8')

def decode_ctype(x):
    return str(ctypes.c_char_p(x).value.decode('utf-8'))



# my_str = convert_ctype('hello world')
# result = stackCal.sum(my_str)