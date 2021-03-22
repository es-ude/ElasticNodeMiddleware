import requests
import os

projectPath = "/path/to/MyProject/"


def replace_ports(port_to):
    port_configs_file_read = open(projectPath + "uploadScripts/portConfigs.py", "r")
    port_configs_content = port_configs_file_read.read()
    port_configs_file_read.close()

    start = port_configs_content.find("portTo" + port_to)
    if start != (-1):
        while port_configs_content[start] != '"':
            start += 1
        end = start + 1
        while port_configs_content[end] != '"':
            end += 1

        current_port = port_configs_content[start + 1:end]

        new_port = input("Port to " + port_to + " (current/default: '" + current_port + "'): ") or current_port

        new_port_configs_content = port_configs_content.replace(current_port, new_port)

        port_configs_file_write = open(projectPath + "uploadScripts/portConfigs.py", "w")
        port_configs_file_write.write(new_port_configs_content)
        port_configs_file_write.close()

        if current_port != new_port:
            print("Port to " + port_to + " set in uploadScripts/portConfigs.py")

            if port_to == "Programmer":
                user_bazelrc_file_read = open(projectPath + "user.bazelrc", "r")
                user_bazelrc_content = user_bazelrc_file_read.read()
                user_bazelrc_file_read.close()

                new_user_bazelrc_content = user_bazelrc_content.replace(current_port, new_port)

                if new_user_bazelrc_content != user_bazelrc_content:
                    user_bazelrc_file_write = open(projectPath + "user.bazelrc", "w")
                    user_bazelrc_file_write.write(new_user_bazelrc_content)
                    user_bazelrc_file_write.close()
                    print("Port to " + port_to + " set in user.bazelrc")
                else:
                    print("Could not find port to ", port_to, "in user.bazelrc")

    else:
        print("Could not find port to", port_to, "in portConfigs.py")


def create_files(directory, file_name):
    if directory != "":
        directory = directory + "/"

    if not os.path.isfile(projectPath + directory + file_name):

        print("Getting", file_name, "...")
        file_content = requests.get("https://raw.githubusercontent.com/es-ude/ElasticNodeMiddleware/master/templates/" +
                                    file_name).text

        if file_name == "bitfileConfigs.py":
            file_content = file_content.replace("../bitfiles/.bit", projectPath + "bitfiles/bitfile.bit")

        print("Writing", file_name, "...")
        file_write = open(projectPath + directory + file_name, "w")
        file_write.write(file_content)
        file_write.close()

        print("Created", file_name)


if __name__ == "__main__":
    create_files(directory="", file_name="user.bazelrc")
    create_files(directory="uploadScripts", file_name="portConfigs.py")
    create_files(directory="uploadScripts", file_name="bitfileConfigs.py")

    replace_ports("Programmer")
    replace_ports("Elasticnode")
