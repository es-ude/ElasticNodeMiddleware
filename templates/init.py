def replacePorts(portTo):
    r  = open("uploadScripts/portConfigs.py", "r")
    f = r.read()
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
        r.close()
        if portTo == "Programmer":
            r = open("user.bazelrc", "r")
            f = r.read()
            w = open("user.bazelrc", "w")
            newText = f.replace(current,newPort)
            w.write(newText)
            if newText != f:
                print("Port to " + portTo + " set in user.bazelrc")
            w.close()
            r.close()
    else:
        print("Could not find port to " + portTo)

replacePorts("Programmer")
replacePorts("Elasticnode")