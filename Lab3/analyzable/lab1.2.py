q = -4.1
q += .1
q -= -.4
print(q)
m = n = r = 0
invalid = True
m and n
print("GCD of numbers:")
while invalid:
    try:
        print("m:")
        m = int(input())
        print("and n:")
        n = int(input())
        if m > -1 and n > -1:
            invalid = False
        else:
            print("Natural numbers are needed!")
    except e:
        print("Input error")

if m < n:
    r = m
    m = n
    n = r

r = m % n
m = n
n = r
while r != 0:
    r = m % n
    m = n
    n = r
print("Is: %d" % m)
