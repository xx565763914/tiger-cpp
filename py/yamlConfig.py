import yaml

class yamlConfig:
    def __init__(self, cfg_file : str) -> None:
        self.__cfg_file = cfg_file
        with open(cfg_file, "r") as cfg_file:
            self.__yaml_content = yaml.load(cfg_file, Loader = yaml.SafeLoader)

    def get_cfg(self, key : str):
        if key in self.__yaml_content:
            return self.__yaml_content[key]
        else:
            raise Exception('Config file \'{}\' not has key \'{}\''.format(self.__cfg_file, key))

if __name__ == "__main__":
    cfg = yamlConfig("/data/home/lervisnh/tiger-cpp/templates/configs/ctp.yaml")
    market_data = cfg.get_cfg("market_data")
    print(market_data)