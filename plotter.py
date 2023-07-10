import pandas as pd
import matplotlib.pyplot as plt
import csv

seq_times = [0.00012, 0.10353, 0.06503, 0.04207, 112.38500]

def write_to_csv(filename, data):
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['x', 'y'])  # Write header row
        writer.writerows(data)  # Write data rows

def get_speedup(x, par_times, n):
    c = 0
    if n == '5':
        c = 0
    elif n == '11':
        c = 1
    elif n == '12':
        c = 2
    elif n == '15':
        c = 3
    elif n == '17':
        c = 4
    ans = []
    for i in range(len(par_times)):
        ans.append([x[i], seq_times[c]/par_times[i]])
    write_to_csv(n+'s.csv', ans)


files = ['5', '11', '12', '15', '17']
plt.figure()
for i in files:
    data = pd.read_csv(i + '.csv')
    x = data['x']
    y = data['y']
    get_speedup(x, y, i)
    plt.plot(x, y,marker='*', markersize=5, label= i + " nodos")

plt.xlabel('Numero de hilos')
plt.ylabel('Tiempo')

plt.yscale('log')
plt.legend(loc='upper right')
plt.savefig('tiempoejecución.png')

plt.figure()
for i in files:
    data = pd.read_csv(i + 's.csv')
    x = data['x']
    y = data['y']
    plt.plot(x, y,marker='*', markersize=5, label= i + " nodos")

plt.xlabel('Numero de hilos')
plt.ylabel('SpeedUp')

plt.legend(loc='upper right')
plt.savefig('speedUp.png')