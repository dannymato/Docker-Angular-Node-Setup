import csv
import time


while True:
    print('#'*5 + 'New Data' + '#'*5)
    output = []
    with open('/tmp/netfilterlog', 'r') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')

        for i in reader:
            found = False
            for x in output:
                if x[0] == i:
                    x[1] += 1
                    found = True
                    break
            if not found:
                
                output.append([i, 1])
        
        for x in output:
            print(x)

    time.sleep(5)
            