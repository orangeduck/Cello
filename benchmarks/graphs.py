import numpy as np
from matplotlib import pyplot as plt

languages = np.array([
    'C', 'C++', 'Cello', 
    'Java', 'Javascript', 'Python', 
    'Ruby', 'Lua', 'Lua JIT'])

experiments = [
  'Array', 'Map', 'N-Bodies', 
  'Dictionary', 'Sudoku', 'Matrix', 
  'Garbage Collection']
    
results_array = np.array([
    0.02, 0.02, 0.10,
    0.14, 0.07, 0.11, 
    0.07, 2.34, 0.24])
    
results_map = np.array([
    0.24, 0.05, 0.54,
    0.60, 1.92, 9.73,
    1.24, 5.46, 2.01])

results_nbodies = np.array([
    0.01, 0.01, 0.07,
    0.07, 0.09, 1.52,
    1.45, 0.66, 0.02])

results_dict = np.array([
    0.09, 0.13, 0.25,
    0.24, 0.46, 0.18,
    0.44, 0.23, 0.18])

results_sudoku = np.array([
    0.14, 0.14, 0.15,
    0.29, 0.44, 5.30,
    9.64, 6.34, 0.49])

results_matrix = np.array([
    0.03,  0.01,  0.02,
    0.11,  0.23,  2.33,
    7.62,  0.94,  0.03])

results_gc = np.array([
    0.01,  0.01,  0.26,
    0.06,  0.25,  3.34,
    5.37,  8.02,  0.31])

results = [
    results_array, results_map,    results_nbodies,
    results_dict,  results_sudoku, results_matrix,
    results_gc]
    
#cols = [
#    '#006666', '#FF6600', '#991F00',
#    '#339933', '#009999', '#006666',
#    '#FF6600', '#991F00', '#339933']
    
cols = [
    '#006666', '#FF6600', '#991F00',
    '#006666', '#FF6600', '#991F00',
    '#006666', '#FF6600', '#991F00']
    
ylims = [
    3.25, 13.5, 2.2,
    0.65, 13.5, 11, 11
]
    
for exp, result, ylim in zip(experiments, results, ylims):
    
    fig = plt.figure(figsize=(3, 2.5))
    ax = fig.add_subplot(1, 1, 1, axisbg='#FCFCFC')
    
    bars = ax.bar(np.arange(len(result)), result)
    
    for bar, col in zip(bars, cols):
        bar.set_color(col)
        bar.set_edgecolor('#555555')
        height = bar.get_height()
        ax.text(
          bar.get_x()+bar.get_width()/1.5, 
          height + (ylim/20), '%0.2f' % height, 
          ha='center', va='bottom', rotation='vertical', 
          color='#555555')
            
    plt.xticks(np.arange(len(result)) + 0.75 / 2, languages, rotation='vertical')
    
    plt.gca().xaxis.grid(False)
    plt.gca().yaxis.grid(False)
    plt.ylim((0, ylim))
    plt.tight_layout()
    plt.show()
        
    
