import os
import requests
from sys import argv
from os import remove

projectName = "projectName"
portToProgrammer = input("Port to Programmer (default: /dev/ttyACM0): ") or "/dev/ttyACM0"
portToElasticnode = input("Port to Elastic node (default: /dev/ttyACM1): ") or "/dev/ttyACM1"

def cloneFile(dir, file):
    f = open(dir+file, "w")
    raw = requests.get(link+file).text
    raw = raw.replace("MyProject",projectName)
    raw = raw.replace("/dev/ttyACM0",portToProgrammer)
    raw = raw.replace("/dev/ttyACM1",portToElasticnode)
    raw = raw.replace("../bitfiles/.bit",os.path.abspath("")+"/bitfiles/bitfile.bit")
    f.write(raw)
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
cloneFile("app/","blinkExample.c")
cloneFile("","WORKSPACE")
cloneFile("","BUILD.bazel")


f = open("app/BUILD.bazel", "w")
f.write(requests.get(link+"appBUILD.bazel").text.replace("/dev/ttyACM0",portToProgrammer))
f.close()

print("TO-DO: Check versions in WORKSPACE.")

### Auto delete ###
i = input("Delete this file? (y/n) (default y) ")
if  i != "n" and i != "no":
    remove(argv[0])
