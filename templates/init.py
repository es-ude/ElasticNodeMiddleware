import os
import requests

projectName = "projectName"
portToProgrammer = input("Port to Programmer (default: /dev/ttyACMO): ") or "/dev/ttyACMO"
portToElasticnode = input("Port to Elastic node (default: /dev/ttyACM1): ") or "/dev/ttyACM1"

def cloneFile(dir, file):
    f = open(dir+file, "w")
    git = requests.get(link+file).text
    git.replace("MyProject",projectName)
    git.replace("/dev/ttyACMO",portToProgrammer)
    git.replace("/dev/ttyACM1",portToElasticnode)
    git.replace("../bitfiles/.bit",os.path.realpath(__file__)+"/bitfiles/.bit")
    f.write(git)
    f.close()

print("Cloning files from es-ude/ElasticNodeMiddleware...")

os.mkdir("scripts")
os.mkdir("bitfiles")

######remove branch form link
#link = "https://raw.githubusercontent.com/es-ude/ElasticNodeMiddleware/master/templates/"
link = "https://raw.githubusercontent.com/es-ude/ElasticNodeMiddleware/ownProgramInit/templates/"

pyfiles = ["uploadBitfiles.py", "portConfigs.py", "bitfileConfigs.py"]
for file in pyfiles:
    cloneFile("scripts/",file)

cloneFile("app/","main.c")
cloneFile("","WORKSPACE")
cloneFile("","BUILD.bazel")

f = open("app/BUILD.bazel", "w")
f.write(requests.get(link+"appBUILD.bazel").text.replace("/dev/ttyACMO",portToProgrammer))
f.close()

print("TO-DO:")
print("Add bitfile names in scripts/bitfilesConfigs.py.")
print("Check versions in WORKSPACE.")

### Auto delete ###
from os import remove
from sys import argv
remove(argv[0])
