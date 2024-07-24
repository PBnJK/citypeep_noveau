# Converts an actor XML to a .act file

import sys
import xml.etree.ElementTree as et


ONE: int = 4096

PATH_IDX: int = 0
ROT_IDX: int = 1
TRANS_IDX: int = 2
SCALE_IDX: int = 3


class Actor:
    def __init__(self, args: list[str]) -> None:
        self.name: str = args[1]
        self.output: str = self.name[:-4] + ".act"

        self.old_path: str = ""
        self.old_size: int = 0

        self.flags: int = 0

        # List of lists
        # [
        #  ["PATH/TO/MESH1", [rot], [trans], [scale]],
        #  ["PATH/TO/MESH2", [rot], [trans], [scale]],
        #  ...
        # ]
        self.models = []

        root = et.parse(self.name).getroot()
        for child in root:
            if child.tag == "model":
                self.__parse_model(child)

    @staticmethod
    def __parse_vec(vec: str) -> int:
        return [int(i) for i in vec.split(" ")]

    def __parse_model(self, model) -> None:
        data = [
            "../model/" + model.attrib["path"],
            [0, 0, 0],
            [0, 0, 0],
            [ONE, ONE, ONE],
        ]

        for child in model:
            if child.tag == "rot":
                data[ROT_IDX] = self.__parse_vec(child.text)
            elif child.tag == "trans":
                data[TRANS_IDX] = self.__parse_vec(child.text)
            elif child.tag == "scale":
                data[SCALE_IDX] = self.__parse_vec(child.text)

        self.models.append(data)

    def __save_header(self, file) -> None:
        print("1. Writing header:")

        self.mcount: int = len(self.models)

        file.write(self.flags.to_bytes(4, byteorder="little", signed=False))
        print(f"* Wrote {self.flags}, 4 bytes")

        file.write(self.mcount.to_bytes(4, byteorder="little", signed=False))
        print(f"* Wrote {self.mcount}, 4 bytes\n")

    @staticmethod
    def __save_vector(file, vec: list[int], size: int) -> None:
        for item in vec:
            file.write(item.to_bytes(size, byteorder="little", signed=True))

    def __save_models(self, file) -> None:
        print("2. Writing models:")

        for model in self.models:
            if model[PATH_IDX] == self.old_path:
                file.write(0xFFFFFFFF.to_bytes(4, byteorder="little", signed=False))
                print("* Wrote 0xFFFFFFFF (repeat model)")
            else:
                with open(model[PATH_IDX], "rb") as f:
                    file.write(f.read())

                self.old_path = model[PATH_IDX]
                print(f"* Wrote model @ {model[PATH_IDX]}")

            self.__save_vector(file, model[ROT_IDX], 2)
            print(f"* Wrote rotation {model[ROT_IDX]}")

            file.write(b"\x00\x00")

            self.__save_vector(file, model[TRANS_IDX], 4)
            print(f"* Wrote translation {model[TRANS_IDX]}")

            self.__save_vector(file, model[SCALE_IDX], 4)
            print(f"* Wrote scale {model[SCALE_IDX]}\n")

    def save(self) -> None:
        with open(self.output, "wb") as f:
            self.__save_header(f)
            self.__save_models(f)


actor: Actor = Actor(sys.argv)
actor.save()
