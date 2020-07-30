from scripts.Configuration import Configuration

# define your address for your bitfiles
S15_ADDRESS_1 =0x0
S15_ADDRESS_2 =0x90000

DUMMY_ADDRESS = 0x0

class BitfileConfigs:

    s15ConfigPart1 = None
    s15ConfigPart2 = None
    dummyConfig = None

    def __init__(self):
        # create a configuration for your bitfiles
        # for bazel run you have to write the absolute path
        # self.s15ConfigPart1 = Configuration("../bitfiles/s15_p1.bit", S15_ADDRESS_1, S15_ADDRESS_1)
        # self.s15ConfigPart2 = Configuration("../bitfiles/s15_p2.bit", S15_ADDRESS_2, S15_ADDRESS_2)

        # change to your absolute path to the bitfiles!
        self.s15ConfigPart1 = Configuration("/home/annika/projects-es/ElasticNodeMiddleware/bitfiles/s15_p1.bit", S15_ADDRESS_1, S15_ADDRESS_1)
        self.s15ConfigPart2 = Configuration("/home/annika/projects-es/ElasticNodeMiddleware/bitfiles/s15_p2.bit", S15_ADDRESS_2, S15_ADDRESS_2)
        self.dummyConfig = Configuration("/home/annika/projects-es/ElasticNodeMiddleware/bitfiles/dummy.bit", DUMMY_ADDRESS, DUMMY_ADDRESS)