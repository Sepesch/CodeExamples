import matplotlib.pyplot as plt
def Rabin_Karp(text, pattern, d=101, q=256):
    """Реализует алгоритм поиска подстроки в строке с использованием метода Рабина-Карпа."""
    text_len = len(text)
    pat_len = len(pattern)

    pat_hash = ord(pattern[0])
    text_hash = ord(text[0])
    for i in range(pat_len-1):
        pat_hash = pat_hash*q+ord(pattern[i+1])
        text_hash = text_hash * q + ord(text[i + 1])
    pat_hash = pat_hash%d
    text_hash = text_hash%d

    result = []
    for i in range(text_len - pat_len + 1):
        if pat_hash == text_hash:
            if pattern == text[i:i+pat_len]:
                result.append(i)
        if i < text_len - pat_len:
            text_hash = ((text_hash - ord(text[i]) * q ** (pat_len-1)) * q + ord(text[i + pat_len])) % d
    return result

'''pattern = input()
text = input()
result = Rabin_Karp(text, pattern)
print(*result)'''

def calculate_area(polygon):
    s1, s2 = 0, 0
    for i in range(1, len(polygon)):
        s1 += polygon[i - 1][0] * polygon[i][1]
    s1 += polygon[-1][0] * polygon[0][1]
    for i in range(1, len(polygon)):
        s2 += polygon[i - 1][1] * polygon[i][0]
    s2 += polygon[-1][1] * polygon[0][0]
    return abs(s1 - s2) / 2
def rotate(p1, p2, p3):
    return (p2[0] - p1[0]) * (p3[1] - p1[1]) - (p2[1] - p1[1]) * (p3[0] - p1[0]) >= 0
def graham_scan(points):
    n = len(points)
    indices = list(range(n))

    for i in range(1, n):
        if points[indices[i]][0] < points[indices[0]][0]:
            indices[i], indices[0] = indices[0], indices[i]

    for i in range(2, n):
        j = i
        while j > 1 and not rotate(points[indices[0]], points[indices[j - 1]], points[indices[j]]):
            indices[j], indices[j - 1] = indices[j - 1], indices[j]
            j -= 1

    hull = [indices[0], indices[1]]
    for i in range(2, n):
        while not rotate(points[hull[-2]], points[hull[-1]], points[indices[i]]):
            hull.pop()
        hull.append(indices[i])
    return [points[i] for i in hull]
def visualize(points, convex_hull):
    plt.figure(figsize=(8, 8))

    x, y = zip(*points)
    plt.scatter(x, y, color='blue', label='Точки')

    hull_x, hull_y = zip(*convex_hull)
    hull_x += (hull_x[0],)
    hull_y += (hull_y[0],)
    plt.plot(hull_x, hull_y, color='red', label='Выпуклая оболочка')

    plt.title('Выпуклая оболочка точек')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.legend()
    plt.grid(True)
    plt.show()
n = int(input())
points = [list(map(int, input().split(', '))) for _ in range(n)]
convex_hull = graham_scan(points)
area = calculate_area(convex_hull)
print((convex_hull, area))
visualize(points, convex_hull)