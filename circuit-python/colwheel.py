""" RGB color wheel (use one value instead of three) """

def colwheel(n):
    if n == 0:
        return (0, 0, 0)
    if n > 0 and n <= 85:
        return (0, int(255-n*3), int(3*n))
    if n > 85 and n <= 170:
        n -= 85
        return (int(n*3), 0, int(255-n*3))
    if n > 170 and n <= 255:
        n -= 170
        return (int(255-n*3), int(n*3), 0)