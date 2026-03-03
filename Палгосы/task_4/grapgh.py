import matplotlib.pyplot as plt

# Данные из таблицы
sizes = [64, 128, 256, 512, 1024, 1536, 2048, 2560, 3072]
cpu_times = [0.1, 0.1, 14, 37, 260, 1118, 2931, 8671, 17163]  # 0 мс заменено на 0.1 для отображения
gpu_times = [1, 3, 24, 68, 302, 1107, 2475, 4965, 9576]

# Создаем график
plt.figure(figsize=(10, 6))

# Линии для CPU и GPU
plt.plot(sizes, cpu_times, 'o-', linewidth=2, markersize=8,
         label='CPU (6 потоков)', color='blue')
plt.plot(sizes, gpu_times, 's-', linewidth=2, markersize=8,
         label='GPU (OpenCL)', color='red')

# Настройки графика
plt.xlabel('Размер матрицы (N×N)', fontsize=12)
plt.ylabel('Время выполнения (мс)', fontsize=12)
plt.title('Сравнение производительности CPU и GPU\nдля умножения матриц', fontsize=14, fontweight='bold')
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)

plt.tight_layout()
plt.show()