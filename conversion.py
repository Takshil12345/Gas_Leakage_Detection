import pandas as pd

import csv

for i in range(1, 11):
    with open('batch'+str(i)+'.dat', 'r') as dat_file:
        with open('Batch'+str(i)+'e.csv', 'w', newline='') as csv_file:
            writer = csv.writer(csv_file)
            for line in dat_file:
                data = line.strip().split(' ')
                row = [data[0]]
                for d in data[1:]:
                    col, val = d.split(':')
                    row.append(val)
                writer.writerow(row)
