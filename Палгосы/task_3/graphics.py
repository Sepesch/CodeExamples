import matplotlib.pyplot as plt
import numpy as np

# Данные из вывода сортировки
sizes = [10000, 50000, 90000, 130000, 170000, 210000, 250000, 290000, 330000, 370000,
         410000, 450000, 490000, 530000, 570000, 610000, 650000, 690000, 730000, 770000,
         810000, 850000, 890000, 930000, 970000]

seq_times = [1.96, 10.75, 17.03, 26.31, 32.31, 40.65, 50.80, 59.18, 69.67, 74.98,
             85.92, 91.94, 100.63, 109.34, 119.20, 129.78, 138.71, 145.35, 156.33, 165.57,
             171.93, 182.82, 191.38, 202.96, 208.39]

par_times = [1.68, 2.65, 4.51, 6.85, 7.38, 10.03, 10.85, 13.67, 14.13, 15.67,
             18.89, 20.25, 22.11, 26.38, 28.10, 30.81, 28.77, 29.83, 33.49, 34.84,
             36.01, 39.77, 42.77, 42.81, 42.89]

fig, ax = plt.subplots(figsize=(10, 6))

# График времени выполнения
ax.plot(sizes, seq_times, 'o-', linewidth=2, markersize=6, label='Последовательная сортировка')
ax.plot(sizes, par_times, 's-', linewidth=2, markersize=6, label='Параллельная сортировка (12 потоков)')

ax.set_xlabel('Размер массива', fontsize=12)
ax.set_ylabel('Время выполнения (мс)', fontsize=12)
ax.set_title('Сравнение алгоритмов быстрой сортировки', fontsize=14, fontweight='bold')
ax.grid(True, alpha=0.3, linestyle='--')
ax.legend(fontsize=11, loc='upper left')
ax.tick_params(axis='both', which='major', labelsize=10)

# Форматируем подписи оси X для отображения фактических значений
ax.set_xticks(sizes[::5])  # показываем каждую 5-ю метку для читаемости
ax.set_xticklabels([f'{size//1000}K' for size in sizes[::5]], rotation=45, ha='right')

plt.tight_layout()
plt.savefig('sorting_comparison.png', dpi=300, bbox_inches='tight')
plt.show()