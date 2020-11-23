import requests
import sys

def replacePorts(portTo):
    pC  = open("uploadScripts/portConfigs.py", "r")
    f = pC.read()
    pC.close()
    st = f.find("portTo" + portTo)
    if st != (-1):
        while f[st] != '"':
            st += 1
        en = st + 1
        while f[en] != '"':
            en += 1
        current = f[st+1:en]
        w = open("uploadScripts/portConfigs.py", "w")
        newPort = input("Port to " + portTo + " (current: " + current + " ): ") or current
        newText = f.replace(current,newPort)
        w.write(newText)
        if newText != f:
            print("Port to " + portTo + " set in uploadScripts/portConfigs.py")
        w.close()
        if portTo == "Programmer":
            r = open("user.bazelrc", "r")
            f = r.read()
            r.close()
            w = open("user.bazelrc", "w")
            newText = f.replace(current,newPort)
            if newText != f:
                w.write(newText)
                print("Port to " + portTo + " set in user.bazelrc")
            else:
                if input("Could not find port " + portTo + " in user.bazelrc. Load default user.bazelrc? (y/N) (default N) ") == "y" or "yes":
                    w.write("run -- " + newPort)
                    print("Loaded default user.bazelrc and set port to " + portTo + ".")
            w.close()
    else:
        if input("Could not find port to " + portTo + ". Load default portConfigs? (y/N) (default N) ") == "y" or "yes":
            w = open("uploadScripts/portConfigs.py", "w")
            w.write(requests.get("https://raw.githubusercontent.com/es-ude/ElasticNodeMiddleware/master/templates/portConfigs.py").text)
            w.close
            print("Successful, run again to change ports.")
            sys.exit()

replacePorts("Programmer")
replacePorts("Elasticnode")
