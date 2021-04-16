from scripts.Configuration import Configuration

# define your address for your bitfiles
ADDRESS_1 = 0x0
ADDRESS_2 = 0x90000


class BitfileConfigs:
    ConfigPart1 = None
    ConfigPart2 = None

    def __init__(self):
        self.ConfigPart1 = Configuration("../bitfiles/.bit", ADDRESS_1, ADDRESS_1)
        # comment in when uploading two bitfiles:
        # self.ConfigPart2 = Configuration("../bitfiles/.bit", ADDRESS_2, ADDRESS_2)
