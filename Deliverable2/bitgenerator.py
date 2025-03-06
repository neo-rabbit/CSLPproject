import random
import sys

if len(sys.argv) > 2:
    n = sys.argv[2]
else:
    n = 10000

f = open(sys.argv[1], 'a')
for i in range(int(n), 0, -1):
    f.write(str(random.randrange(0,2)))
f.close()