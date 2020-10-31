
import os
import requests

projectName = "MyProject"
portToProgrammer = input("Port to Programmer (default: /dev/ttyACMO): ") or "/dev/ttyACMO"
portToElasticnode = input("Port to Elastic node (default: /dev/ttyACM1): ") or "/dev/ttyACM1"

def cloneFile(dir, file):
    f = open(dir+file, "w")
    git = requests.get(link+file).text
    git.replace("OwnPro",projectName)
    git.replace("/dev/ttyACMO",portToProgrammer)
    git.replace("/dev/ttyACM1",portToElasticnode)
    f.write(git)
    f.close()

print("Cloning files from es-ude/ElasticNodeMiddleware...")

os.mkdir("scripts")

######remove branch form link
#link = "https://raw.githubusercontent.com/es-ude/ElasticNodeMiddleware/master/templates/"
link = "https://raw.githubusercontent.com/es-ude/ElasticNodeMiddleware/ownProgramInit/templates/"

pyfiles = ["uploadBitfiles.py", "portConfigs.py", "bitfileConfigs.py"]

for file in pyfiles:
    cloneFile("scripts/",file)

f = open("app/BUILD.bazel", "w")
f.write(requests.get(link+"appBUILD.bazel").text)
f.close()

cloneFile("app/","main.c")
cloneFile("","WORKSPACE")
cloneFile("","BUILD.bazel")

print("TO-DO:")
print("Change path to bitfiles in scripts/bitfilesConfigs.py.")
print("Check versions in WORKSPACE.")

print(os.path.realpath(__file__))
### Auto delete
from os import remove
from sys import argv
#remove(argv[0])
