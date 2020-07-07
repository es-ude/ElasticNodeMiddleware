from Configuration import Configuration

class BitfileConfigs:

    # define your address for your bitfiles
    S15_ADDRESS_1 =0x0
    S15_ADDRESS_2 =0x90000

    def configurations(self):
        # create a configuration for your bitfiles
        self.s15ConfigPart1 = Configuration("../bitfiles/s15_p1.bit", BitfileConfigs.S15_ADDRESS_1, BitfileConfigs.S15_ADDRESS_1)
        self.s15ConfigPart2 = Configuration("../bitfiles/s15_p2.bit", BitfileConfigs.S15_ADDRESS_2, BitfileConfigs.S15_ADDRESS_2)