# Converts an actor XML to a .act file

import sys
import xml.etree.ElementTree as et


ONE: int = 4096

ROT_IDX: int = 0
TRANS_IDX: int = 1
SCALE_IDX: int = 2
PATH_IDX: int = 3
HPATH_IDX: int = 4
HNAME_IDX: int = 5


class Actor:
    def __init__(self, args: list[str]) -> None:
        self.name: str = args[1]

        self.output: str = self.name[:-4] + ".act"
        self.h_output: str = f"../data/act_{self.name[:-4]}.h"

        self.h_name: str = self.name[:-4].replace("/", "_")
        self.h_guard: str = f"GUARD_CITYPEEP_DATA_ACTOR_{self.h_name.upper()}_H_"

        self.old_path: str = ""
        self.old_size: int = 0

        self.flags: int = 0

        self.has_m: bool = False
        self.has_h: bool = False

        self.used_hs = []

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

    def __parse_model(self, model) -> list[int]:
        data = [
            [0, 0, 0],
            [0, 0, 0],
            [ONE, ONE, ONE],
        ]

        if "path" in model.attrib:
            data.append("../model/" + model.attrib["path"])
            self.has_m = True
        else:
            data.append("")

        if ("hpath" in model.attrib) and model.attrib["hpath"] not in self.used_hs:
            data.append(model.attrib["hpath"])
            self.used_hs.append(model.attrib["hpath"])
            self.has_h = True
        else:
            data.append("")

        if "hname" in model.attrib:
            data.append(model.attrib["hname"])
            self.has_h = True
        else:
            data.append("")

        for child in model:
            if child.tag == "rot":
                data[ROT_IDX] = self.__parse_vec(child.text)
            elif child.tag == "trans":
                data[TRANS_IDX] = self.__parse_vec(child.text)
            elif child.tag == "scale":
                data[SCALE_IDX] = self.__parse_vec(child.text)
            elif child.tag == "model":
                self.__parse_model(child)

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
        if not self.has_m:
            print("No .m, can't make binary")
            return

        with open(self.output, "wb") as f:
            self.__save_header(f)
            self.__save_models(f)

    def __hsave_header(self, file) -> None:
        print("1. Writing header:")

        file.write("\t.rot={0, 0, 0},\n")
        file.write("\t.trans={0, 0, 0},\n")
        file.write("\t.scale={ONE, ONE, ONE},\n\n")

        file.write("\t.flags={\n")
        file.write("\t\t.active=1,\n")
        file.write("\t\t.visible=1\n")
        file.write("\t},\n")

    @staticmethod
    def __hsave_vector(file, vec: list[int], size: int) -> None:
        file.write("{")

        for item in vec:
            file.write(f"{item}, ")

        file.write("0}")

    def __hsave_models(self, file) -> None:
        print("2. Writing models:")

        self.mcount: int = len(self.models)
        file.write(f"\t.meshCount={self.mcount},\n")
        file.write("\t.mesh={\n")

        for model in self.models:
            file.write(f"\t\t{model[HNAME_IDX]},\n")

        file.write("\t},\n")

    @staticmethod
    def __cify(vec: list[int]) -> str:
        return f"{{ {vec[0]}, {vec[1]}, {vec[2]} }}"

    def save_to_header(self) -> None:
        if not self.has_h:
            print("No .h, can't make header")
            return

        with open(self.h_output, "w") as f:
            f.write(f"#ifndef {self.h_guard}\n")
            f.write(f"#define {self.h_guard}\n\n")

            for model in self.models:
                if model[HPATH_IDX] != "":
                    f.write(f'#include "{model[HPATH_IDX]}"\n\n')

            f.write('#include "actor.h"\n\n')

            f.write(f"static CP_Actor act_{self.h_name} = {{\n")

            self.__hsave_header(f)
            self.__hsave_models(f)

            f.write("};\n\n")

            for count, model in enumerate(self.models):
                f.write(
                    f"static const SVECTOR R_{count} = {self.__cify(model[ROT_IDX])};\n"
                )
                f.write(
                    f"static const VECTOR T_{count} = {self.__cify(model[TRANS_IDX])};\n"
                )
                f.write(
                    f"static const VECTOR S_{count} = {self.__cify(model[SCALE_IDX])};\n\n"
                )

            f.write(f"#endif // !{self.h_guard}\n")


actor: Actor = Actor(sys.argv)
actor.save()
actor.save_to_header()
