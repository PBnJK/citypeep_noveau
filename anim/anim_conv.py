# Converts animation XML into .ANI files

import sys
import xml.etree.ElementTree as et


ROT: int = 1
TRANS: int = 2
SCALE: int = 4


class Action:  # lawsuit
    def __init__(self, index: int) -> None:
        self.bone_index: int = index
        self.kf_type: int = 0

        self.rot: list[str] = [0, 0, 0]
        self.trans: list[str] = [0, 0, 0]
        self.scale: list[str] = [0, 0, 0]

    @staticmethod
    def __parse_vec(vec: str) -> int:
        return [int(i) for i in vec.split(" ")]

    def add_rot(self, rot: str) -> None:
        self.rot = self.__parse_vec(rot)
        self.kf_type |= ROT

    def add_trans(self, trans: str) -> None:
        self.trans = self.__parse_vec(trans)
        self.kf_type |= TRANS

    def add_scale(self, scale: str) -> None:
        self.scale = self.__parse_vec(scale)
        self.kf_type |= SCALE

    @staticmethod
    def __save_vec(f, vec: list[int], size: int) -> None:
        for item in vec:
            f.write(item.to_bytes(size, byteorder="little", signed=True))

    def save(self, f) -> None:
        print("\n* Writing action:")

        f.write(self.bone_index.to_bytes(1, byteorder="little", signed=False))
        print(f"* * Saved bone index {self.bone_index}")

        f.write(self.kf_type.to_bytes(1, byteorder="little", signed=False))
        print(f"* * Saved kf type {self.kf_type}")

        f.write(b"\x00\x00")
        print("* * Wrote 0000 (padding), 2 bytes")

        self.__save_vec(f, self.rot, 2)
        print(f"* * Saved rot: {self.rot}")

        f.write(b"\x00\x00")

        self.__save_vec(f, self.trans, 4)
        print(f"* * Saved trans: {self.trans}")

        self.__save_vec(f, self.scale, 4)
        print(f"* * Saved scale: {self.scale}")

    def __str__(self) -> str:
        text: str = f"idx: {self.bone_index}, type: {self.kf_type}\n"

        if self.kf_type & ROT:
            text += f"- rot: {self.rot}\n"

        if self.kf_type & TRANS:
            text += f"- trans: {self.trans}\n"

        if self.kf_type & SCALE:
            text += f"- scale: {self.scale}"

        return text

    def __repr__(self) -> str:
        return self.__str__()


class Anim:
    def __init__(self, args: list[str]) -> None:
        self.name: str = args[1]
        self.output: str = self.name[:-4] + ".ani"

        self.frames: list[list[Action]] = []

        root = et.parse(self.name).getroot()

        if "res" in root.attrib:
            self.res: int = int(root.attrib["res"])
        else:
            self.res: int = 30

        for child in root:
            if child.tag == "frame":
                self.__parse_frame(child)

    def __parse_frame(self, frame) -> None:
        this_frame: list[Action] = []

        for target in frame:
            action: Action = Action(int(target.attrib["idx"]))

            for keyframe in target:
                if keyframe.tag == "rot":
                    action.add_rot(keyframe.text)
                elif keyframe.tag == "trans":
                    action.add_trans(keyframe.text)
                elif keyframe.tag == "scale":
                    action.add_scale(keyframe.text)

            this_frame.append(action)

        self.frames.append(this_frame)

    def __save_header(self, f) -> None:
        print("1. Writing header:")

        f.write(self.res.to_bytes(2, byteorder="little", signed=False))
        print(f"* Wrote {self.res}, 2 bytes")

        f.write(b"\x00\x00")
        print("* Wrote 0000 (padding), 2 bytes")

        self.fcount: int = len(self.frames)

        f.write(self.fcount.to_bytes(4, byteorder="little", signed=False))
        print(f"* Wrote {self.fcount}, 4 bytes")

    def __save_frames(self, f) -> None:
        print("\n2. Writing frames:")

        for frame in self.frames:
            f.write(len(frame).to_bytes(4, byteorder="little", signed=False))
            print(f"\n* Wrote action count {len(frame)}, 4 bytes")

            for action in frame:
                action.save(f)

    def save(self) -> None:
        with open(self.output, "wb") as f:
            self.__save_header(f)
            self.__save_frames(f)


anim: Anim = Anim(sys.argv)
anim.save()
