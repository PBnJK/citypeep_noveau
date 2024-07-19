# .obj converter

import sys


class Mesh:
    def __init__(self, name: str) -> None:
        self.verts: list[list[int]] = []
        self.faces: list[list[int]] = []

        self.name: str = name
        self.output: str = name[:-4] + ".mf"

    @staticmethod
    def __psx_vert(v: str) -> int:
        return int(float(v) * 32.0)

    def add_vert(self, v_list: list[str]) -> None:
        self.verts.append([self.__psx_vert(v) for v in v_list])

    @staticmethod
    def __psx_face(f: str) -> int:
        return int(f) - 1

    def add_face(self, f_list: list[str]) -> None:
        if "//" in f_list[0]:
            f_list = [v.split("//")[0] for v in f_list]
        elif "/" in f_list[0]:
            f_list = [v.split("/")[0] for v in f_list]

        if len(f_list) == 4:
            self.faces.append(
                [
                    self.__psx_face(f_list[0]),
                    self.__psx_face(f_list[1]),
                    self.__psx_face(f_list[2]),
                ]
            )
            self.faces.append(
                [
                    self.__psx_face(f_list[0]),
                    self.__psx_face(f_list[2]),
                    self.__psx_face(f_list[3]),
                ]
            )
        else:
            self.faces.append([self.__psx_face(f) for f in f_list])

    def __save_count(self, file) -> None:
        print("1. Saving count...")

        self.vcount = len(self.verts)
        self.fcount = len(self.faces)

        if self.vcount % 2:
            self.verts.append([0, 0, 0])
            self.vcount += 1

        if self.fcount % 2:
            self.faces.append([0, 0, 0])
            self.fcount += 1

        file.write(self.vcount.to_bytes(4, byteorder="little", signed=False))
        print(f" . Wrote vcount {self.vcount}, 4 bytes")

        file.write(self.fcount.to_bytes(4, byteorder="little", signed=False))
        print(f" . Wrote fcount {self.fcount}, 4 bytes")

    def __save_verts(self, file) -> None:
        print("\n2. Saving verts...")

        for v in self.verts:
            file.write(v[0].to_bytes(2, byteorder="little", signed=True))
            file.write(v[1].to_bytes(2, byteorder="little", signed=True))
            file.write(v[2].to_bytes(2, byteorder="little", signed=True))

            print(f" . Wrote {v}, 2 bytes each")

    def __save_faces(self, file) -> None:
        print("\n3. Saving faces...")

        for f in self.faces:
            file.write(f[0].to_bytes(2, byteorder="little", signed=True))
            file.write(f[1].to_bytes(2, byteorder="little", signed=True))
            file.write(f[2].to_bytes(2, byteorder="little", signed=True))

            print(f" . Wrote {f}, 2 bytes each")

    def save(self) -> None:
        print(f"Saving mesh to {self.output}:\n")

        # Refer to dev/format.txt for the format used here
        with open(self.output, "wb") as f:
            self.__save_count(f)
            self.__save_verts(f)
            self.__save_faces(f)


mesh = Mesh(sys.argv[1])

with open(mesh.name) as f:
    for line in f.readlines():
        line = line.strip().split()

        if not line or line[0] == "#":
            continue

        if line[0] == "v":
            mesh.add_vert(line[1:])
        elif line[0] == "f":
            mesh.add_face(line[1:])

mesh.save()
