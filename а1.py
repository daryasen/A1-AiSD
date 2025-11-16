import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib
plt.style.use('default')
matplotlib.rcParams['figure.figsize'] = [12, 8]
matplotlib.rcParams['font.size'] = 12
large_data = pd.read_csv('large_region_data.csv')
small_data = pd.read_csv('small_region_data.csv')
exact_area = 0.25 * np.pi + 1.25 * np.arcsin(0.8) - 1.0
print(f"Точная площадь: {exact_area:.6f}")
plt.figure(figsize=(14, 6))
plt.subplot(1, 2, 1)
plt.plot(large_data['Samples'], large_data['CalculatedArea'], 
         'b-', alpha=0.7, linewidth=1, label='Широкая область (0.5-2.5)')
plt.plot(small_data['Samples'], small_data['CalculatedArea'], 
         'r-', alpha=0.7, linewidth=1, label='Узкая область (1.0-2.0)')
plt.axhline(y=exact_area, color='green', linestyle='--', 
           linewidth=2, label=f'Точное значение ({exact_area:.4f})')
plt.xlabel('Количество точек')
plt.ylabel('Приближенная площадь')
plt.title('Зависимость приближенной площади от количества точек')
plt.legend()
plt.grid(True, alpha=0.3)
plt.xscale('log')
plt.subplot(1, 2, 2)
plt.plot(large_data['Samples'], large_data['CalculatedArea'], 
         'b-', alpha=0.7, linewidth=1, label='Широкая область')
plt.plot(small_data['Samples'], small_data['CalculatedArea'], 
         'r-', alpha=0.7, linewidth=1, label='Узкая область')
plt.axhline(y=exact_area, color='green', linestyle='--', 
           linewidth=2, label=f'Точное значение')
plt.xlabel('Количество точек')
plt.ylabel('Приближенная площадь')
plt.title('Увеличенный вид (линейная шкала)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.xlim(0, 20000)

plt.tight_layout()
plt.savefig('monte_carlo_area_comparison.png', dpi=300, bbox_inches='tight')
plt.show()
plt.figure(figsize=(14, 6))
plt.subplot(1, 2, 1)
plt.plot(large_data['Samples'], large_data['Deviation'], 
         'b-', alpha=0.7, linewidth=1, label='Широкая область')
plt.plot(small_data['Samples'], small_data['Deviation'], 
         'r-', alpha=0.7, linewidth=1, label='Узкая область')
plt.xlabel('Количество точек')
plt.ylabel('Относительная ошибка')
plt.title('Зависимость относительной ошибки от количества точек')
plt.legend()
plt.grid(True, alpha=0.3)
plt.yscale('log')
plt.xscale('log')
plt.subplot(1, 2, 2)
plt.plot(large_data['Samples'], large_data['Deviation'], 
         'b-', alpha=0.7, linewidth=1, label='Широкая область')
plt.plot(small_data['Samples'], small_data['Deviation'], 
         'r-', alpha=0.7, linewidth=1, label='Узкая область')
plt.xlabel('Количество точек')
plt.ylabel('Относительная ошибка')
plt.title('Увеличенный вид (линейная шкала)')
plt.legend()
plt.grid(True, alpha=0.3)
plt.xlim(0, 20000)
plt.ylim(bottom=0)
plt.tight_layout()
plt.savefig('monte_carlo_error_comparison.png', dpi=300, bbox_inches='tight')
plt.show()
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10))
ax1.plot(large_data['Samples'], large_data['CalculatedArea'], 
         'b-', alpha=0.7, linewidth=1, label='Широкая область')
ax1.plot(small_data['Samples'], small_data['CalculatedArea'], 
         'r-', alpha=0.7, linewidth=1, label='Узкая область')
ax1.axhline(y=exact_area, color='green', linestyle='--', 
           linewidth=2, label=f'Точное значение ({exact_area:.4f})')
ax1.set_ylabel('Приближенная площадь')
ax1.set_title('Метод Монте-Карло: оценка площади пересечения кругов')
ax1.legend()
ax1.grid(True, alpha=0.3)
ax2.plot(large_data['Samples'], large_data['Deviation'], 
         'b-', alpha=0.7, linewidth=1, label='Широкая область')
ax2.plot(small_data['Samples'], small_data['Deviation'], 
         'r-', alpha=0.7, linewidth=1, label='Узкая область')
ax2.set_xlabel('Количество точек')
ax2.set_ylabel('Относительная ошибка')
ax2.legend()
ax2.grid(True, alpha=0.3)
ax2.set_yscale('log')


plt.tight_layout()
plt.savefig('monte_carlo_combined.png', dpi=300, bbox_inches='tight')
plt.show()
print("\nСтатистика для N = 100000:")
last_large = large_data.iloc[-1]
last_small = small_data.iloc[-1]
print(f"Широкая область: площадь = {last_large['CalculatedArea']:.6f}, "
      f"ошибка = {last_large['Deviation']:.4f}")
print(f"Узкая область: площадь = {last_small['CalculatedArea']:.6f}, "
      f"ошибка = {last_small['Deviation']:.4f}")
improvement = last_large['Deviation'] / last_small['Deviation']
print(f"\nУзкая область эффективнее широкой в {improvement:.2f} раз")
initial_large_error = large_data.iloc[0]['Deviation']
initial_small_error = small_data.iloc[0]['Deviation']
print(f"\nНачальная ошибка (N=100):")
print(f"Широкая область: {initial_large_error:.4f}")
print(f"Узкая область: {initial_small_error:.4f}")
print(f"\nУменьшение ошибки:")
print(f"Широкая область: в {initial_large_error/last_large['Deviation']:.1f} раз")
print(f"Узкая область: в {initial_small_error/last_small['Deviation']:.1f} раз")
