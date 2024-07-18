# .obj converter

import sys


class Mesh:
    def __init__(self, name: str) -> None:
        self.verts: list[list[int]] = []
        self.vcount: int = 0

        self.faces: list[list[int]] = []
        self.fcount: int = 0

        self.name: str = name
        self.output: str = name[:-4] + ".mf"

    def add_vert(self, v_list: list[str]) -> None:
        self.verts.append([int(float(v) * 32) for v in v_list])
        self.vcount += 1

    def add_face(self, f_list: list[str]) -> None:
        self.faces.append([(int(f) - 1) for f in f_list])
        self.fcount += 1

    def __save_count(self, file) -> None:
        print("1. Saving count...")

        if self.vcount % 2:
            self.verts.append([0, 0, 0])
            self.vcount += 1

        if self.fcount % 2:
            self.faces.append([0, 0, 0])
            self.fcount += 1

        file.write(self.vcount.to_bytes(4, byteorder="little", signed=False))
        print(f" . Wrote vcount {self.vcount}, 4 bytes")

        file.write(self.fcount.to_bytes(4, byteorder="little", signed=False))
        print(f" . Wrote {self.fcount}, 4 bytes")

    def __save_verts(self, file) -> None:
        print("2. Saving verts...")

        for v in self.verts:
            file.write(v[0].to_bytes(2, byteorder="little", signed=True))
            file.write(v[1].to_bytes(2, byteorder="little", signed=True))
            file.write(v[2].to_bytes(2, byteorder="little", signed=True))

            print(f" . Wrote {v}, 2 bytes each")

    def __save_faces(self, file) -> None:
        print("3. Saving faces...")

        for f in self.faces:
            file.write(f[0].to_bytes(2, byteorder="little", signed=True))
            file.write(f[1].to_bytes(2, byteorder="little", signed=True))
            file.write(f[2].to_bytes(2, byteorder="little", signed=True))

            print(f" . Wrote {f}, 2 bytes each")

    def save(self) -> None:
        print("Saving mesh to " + self.output)

        # Refer to dev/format.txt for the format used here
        with open(self.output, "wb") as f:
            self.__save_count(f)
            self.__save_verts(f)
            self.__save_faces(f)


mesh = Mesh(sys.argv[1])

with open(mesh.name) as f:
    for line in f.readlines():
        line = line.strip().split()

        if not line:
            continue

        if line[0] == "v":
            mesh.add_vert(line[1:])
        elif line[0] == "f":
            mesh.add_face(line[1:])

mesh.save()
