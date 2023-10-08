exec_name = "app"
cppcompiler = "clang++"
include_paths = ["/opt/homebrew/Cellar/sfml/2.6.0/include", "3rdparty"]
library_paths = ["/opt/homebrew/Cellar/sfml/2.6.0/lib"]
libraries = ["sfml-graphics", "sfml-window", "sfml-system", "sfml-audio"]
frameworks = []

import os
out_cmd = cppcompiler + " -o build/bin/" + exec_name
for f in os.listdir("src"):
    if f.endswith(".cpp") or f.endswith(".c"):
        out_cmd += " src/" + f
for l in library_paths:
    out_cmd += " -L" + l
for i in include_paths:
    out_cmd += " -I" + i
for l in libraries:
    out_cmd += " -l" + l
for f in frameworks:
    out_cmd += " -framework " + f
os.system(out_cmd)
os.system("./build/bin/" + exec_name)