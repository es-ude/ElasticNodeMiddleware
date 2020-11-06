def replacePorts(portTo):
    r  = open("uploadScripts/portConfigs.py", "r")
    f = r.read()
    st = f.find("portTo" + portTo)
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
        r = open("app/BUILD.bazel", "r")
        f = r.read()
        w = open("app/BUILD.bazel", "w")
        newText = f.replace(current,newPort)
        w.write(newText)
        if newText != f:
            print("Port to " + portTo + " set in app/BUILD.bazel")
        w.close()
        r.close()

replacePorts("Programmer")
replacePorts("Elasticnode")



