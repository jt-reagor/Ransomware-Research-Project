#!/usr/bin/python3
import os
import shutil

print("How many packages?")
numpack = int(input())
print("What difficulty? (1-1)")
diff = int(input())

for i in range(0, numpack):
    fname = "proj"+str(i)
    os.mkdir(fname)
    shutil.copyfile("./malware", fname+"/malware")
    os.chmod(fname+"/malware", 0o777)
    keyseed = bytes.hex(os.urandom(4))
    f = open("./keyseed", "w")
    f.write(keyseed)
    f.close()
    shutil.copyfile("./supersecretfileorig.txt", "./supersecretfile.txt")
    os.system("./malware supersecretfile.txt > /dev/null")
    shutil.move("./keyseed", fname+"/keyseed")
    shutil.move("./supersecretfile.txt", fname+"/supersecretfile.txt")
    shutil.copyfile("./malware.c", fname+"/malware.c")
    os.system("tar -cvf "+fname+".tar.gz ./"+fname+" > /dev/null")
    shutil.rmtree(fname)
