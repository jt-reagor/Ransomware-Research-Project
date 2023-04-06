import os
import shutil
import binascii
import stat

print("How many packages?")
numpack = int(input())
print("What difficulty? (0-2)")
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
    os.system("./malware supersecretfile.txt")
    shutil.move("./keyseed", fname+"/keyseed")
    shutil.move("./supersecretfile.txt", fname+"/supersecretfile.txt")
