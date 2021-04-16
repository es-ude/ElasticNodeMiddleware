from scripts.Configuration import Configuration

# define your address for your bitfiles
ADDRESS_1 = 0x0
ADDRESS_2 = 0x90000


class BitfileConfigs:
    ConfigPart1 = None
    s15ConfigPart2 = None

    def __init__(self):
        # change to your absolute path to the bitfiles!
        self.ConfigPart1 = Configuration("../bitfiles/bitfile.bit", ADDRESS_1,
                                            ADDRESS_1)
        self.ConfigPart2 = Configuration("../bitfiles/bitfile.bit", ADDRESS_2,
                                            ADDRESS_2)
