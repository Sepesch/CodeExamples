import matplotlib.pyplot as plt
import numpy as np
import os

# Новые данные из результатов тестирования
data = {
    'CoarseGrainedList': {
        'threads': [1, 2, 4, 8, 12, 16, 20, 24],
        'throughput': [
            [35053, 31587, 37340, 36695, 29624],
            [33366, 33990, 33720, 33552, 25607],
            [32997, 32252, 36017, 34799, 27291],
            [34159, 35590, 35048, 33354, 26192],
            [32167, 33478, 32187, 31968, 24795],
            [31726, 33354, 31843, 31096, 25730],
            [31911, 31213, 34224, 31065, 25457],
            [31503, 30306, 24807, 28550, 21795]
        ]
    },
    'FineGrainedList': {
        'threads': [1, 2, 4, 8, 12, 16, 20, 24],
        'throughput': [
            [13924, 15578, 15634, 17132, 11987],
            [20893, 20455, 22379, 22303, 14422],
            [38981, 29723, 38594, 37280, 26539],
            [47220, 45686, 45487, 41636, 29788],
            [57181, 57905, 57843, 48250, 36291],
            [43972, 43301, 44816, 49951, 36228],
            [43907, 42145, 39194, 44658, 36118],
            [43591, 43337, 39389, 41425, 35720]
        ]
    },
    'LockFreeList': {
        'threads': [1, 2, 4, 8, 12, 16, 20, 24],
        'throughput': [
            [42581, 43703, 42979, 37696, 156831],
            [62028, 62873, 87232, 85513, 255804],
            [106355, 120094, 131572, 170658, 457645],
            [169609, 180116, 197165, 215152, 617093],
            [211689, 173676, 183762, 213093, 512866],
            [199354, 169658, 193327, 210294, 532821],
            [159732, 152364, 196225, 217218, 547745],
            [206521, 197585, 178781, 223925, 555028]
        ]
    }
}

# Проценты читателей (новые значения)
read_ratios = [0, 25, 50, 75, 100]
read_ratios_labels = [f'{r}%' for r in read_ratios]

# Создаем папку для графиков если ее нет
os.makedirs('graphs_new_data', exist_ok=True)

# Цвета для разных структур
colors = {
    'CoarseGrainedList': 'red',
    'FineGrainedList': 'blue',
    'LockFreeList': 'green'
}

# Стили линий
linestyles = {
    'CoarseGrainedList': '-',
    'FineGrainedList': '--',
    'LockFreeList': '-.'
}

# Маркеры для лучшей различимости
markers = {
    'CoarseGrainedList': 'o',
    'FineGrainedList': 's',
    'LockFreeList': '^'
}

# Создаем 8 графиков - по одному для каждого количества потоков
for i, thread_count in enumerate([1, 2, 4, 8, 12, 16, 20, 24]):
    plt.figure(figsize=(14, 9))

    # Для каждой структуры данных
    for list_name in ['CoarseGrainedList', 'FineGrainedList', 'LockFreeList']:
        # Получаем данные для текущего количества потоков
        thread_index = data[list_name]['threads'].index(thread_count)
        throughput_data = data[list_name]['throughput'][thread_index]

        # Создаем график
        plt.plot(read_ratios, throughput_data,
                 label=list_name,
                 color=colors[list_name],
                 linestyle=linestyles[list_name],
                 linewidth=2.5,
                 marker=markers[list_name],
                 markersize=8,
                 markevery=1)

    # Настройки графика
    plt.title(f'Сравнение производительности списков - {thread_count} поток(ов)',
              fontsize=16, fontweight='bold', pad=20)
    plt.xlabel('Доля читателей (%)', fontsize=14)
    plt.ylabel('Пропускная способность (операций/сек)', fontsize=14)
    plt.legend(fontsize=12, loc='best')
    plt.grid(True, alpha=0.3, linestyle='--')
    plt.xticks(read_ratios, read_ratios_labels, fontsize=11)
    plt.yticks(fontsize=11)

    # Линейная шкала для всех случаев
    plt.ylim(bottom=0)

    # Для случаев с высокими значениями LockFreeList устанавливаем разумные пределы
    if thread_count >= 2:
        max_throughput = max(data['LockFreeList']['throughput'][thread_index])
        plt.ylim(0, max_throughput * 1.1)

    # Улучшаем layout
    plt.tight_layout()

    # Сохраняем график
    filename = f'graphs_new_data/threads_{thread_count:02d}_comparison.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    plt.close()
