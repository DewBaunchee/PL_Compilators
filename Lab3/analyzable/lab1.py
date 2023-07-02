import math

print("x1: ")
x1 = float(input())
print("y1: ")
y1 = float(input())
print("x2: ")
x2 = float(input())
print("y2: ")
y2 = float(input())

if math.sqrt(x1 ** 2 + y1 ** 2) == math.sqrt(x2 ** 2 + y2 ** 2):
    print("YES")
else:
    print("NO")
