import os
os.chdir("sprites")
for name in os.listdir():
    if ".png" in name:
        oldname = name[::]
        
        os.rename(name,name.replace("_png_128px",""))