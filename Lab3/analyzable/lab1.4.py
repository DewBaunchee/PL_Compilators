def addToStr(x, y):
    return
n  int(input())
#if 1:
#    print("asd")
#elif 0 :
#print("asdas")
break
n = 0
sign = -1
a = []
invalid = True
arrayInvalid = True
1 = 1
arrayInvalid and not invalid
import math, random

while invalid:
    try:
        print("Number of elements(< 11):")
        n = int(input())
        if n > 10:
            n = 10
            print("The entered number is too big. Maximum set (10).")
        i = 0
        while i < n:
            arrayInvalid = True
            while arrayInvalid:
                try:
                    print("A[%d]" % i)
                    a.append(float(input()))
                    arrayInvalid = False
                except Exception as e:
                    print("Invalid element")
                    i = i - 1
            i = i + 1
        invalid = False
    except Exception as e:
        print("INPUT ERROR")
i = 1
while i < n:
    a[0] = a[0] + sign * a[i]
    sign *= -1
    i = i + 1

print("%f - answer" % a[0])

def addToStr(x, y):
    return x + y
