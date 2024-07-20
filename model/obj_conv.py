# .obj converter

import sys


class Mesh:
    def __init__(self, args: str) -> None:
        self.verts: list[list[int]] = []
        self.faces: list[list[int]] = []
        self.uvs: list[list[int]] = []
        self.uvidxs: list[list[int]] = []
        self.normals: list[list[int]] = []
        self.nidxs: list[list[int]] = []

        self.name: str = args[1]
        if len(args) == 3:
            self.ext = ".mf"
        else:
            self.ext = ".mt"

        self.output: str = self.name[:-4] + self.ext

    @staticmethod
    def __psx_vert(v: str) -> int:
        return int(float(v) * 32.0)

    def add_vert(self, v_list: list[str]) -> None:
        self.verts.append([self.__psx_vert(v) for v in v_list])

    @staticmethod
    def __psx_face(f: str) -> int:
        return int(f) - 1

    def add_uvidxs(self, t_list: list[str]) -> None:
        if len(t_list) == 4:
            self.uvidxs.append(
                [
                    self.__psx_face(t_list[0]),
                    self.__psx_face(t_list[1]),
                    self.__psx_face(t_list[2]),
                ]
            )
            self.uvidxs.append(
                [
                    self.__psx_face(t_list[0]),
                    self.__psx_face(t_list[2]),
                    self.__psx_face(t_list[3]),
                ]
            )
        else:
            self.uvidxs.append([self.__psx_face(t) for t in t_list])

    @staticmethod
    def __psx_normal(n: str) -> int:
        return int(float(n) * 4096.0)

    def add_nidxs(self, n_list: list[str]) -> None:
        if len(n_list) == 4:
            self.nidxs.append(
                [
                    self.__psx_face(n_list[0]),
                    self.__psx_face(n_list[1]),
                    self.__psx_face(n_list[2]),
                ]
            )
            self.nidxs.append(
                [
                    self.__psx_face(n_list[0]),
                    self.__psx_face(n_list[2]),
                    self.__psx_face(n_list[3]),
                ]
            )
        else:
            self.nidxs.append([self.__psx_face(n) for n in n_list])

    def add_normals(self, n_list: list[str]) -> None:
        self.normals.append([self.__psx_normal(n) for n in n_list])

    def add_face(self, f_list: list[str]) -> None:
        if "//" in f_list[0]:  # TODO: Deal with textureless with normals
            f_list = [v.split("//")[0] for v in f_list]
        elif "/" in f_list[0]:
            self.ext = ".mt"

            f2_list = []
            n_list = []
            t_list = []
            for v in f_list:
                v = v.split("/")
                f2_list.append(v[0])
                t_list.append(v[1])
                n_list.append(v[2])

            f_list = f2_list

            self.add_uvidxs(t_list)
            self.add_nidxs(n_list)

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

    @staticmethod
    def __psx_uv(t: str) -> int:
        return int(float(t) * 4096.0)

    def add_uv(self, t_list: list[str]) -> None:
        self.ext = ".mt"

        self.uvs.append(
            [
                self.__psx_uv(t_list[0]),
                4096 - self.__psx_uv(t_list[1]),
            ]
        )

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

        if self.ext == ".mt":
            self.tcount = len(self.uvs)
            self.ncount = len(self.normals)

            if self.tcount % 2:
                self.tcount.append([0, 0, 0])
                self.tcount += 1

            if self.ncount % 2:
                self.ncount.append([0, 0, 0])
                self.ncount += 1

            file.write(self.tcount.to_bytes(4, byteorder="little", signed=False))
            print(f" . Wrote tcount {self.tcount}, 4 bytes")

            file.write(self.ncount.to_bytes(4, byteorder="little", signed=False))
            print(f" . Wrote ncount {self.ncount}, 4 bytes")

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
            file.write(f[0].to_bytes(2, byteorder="little", signed=False))
            file.write(f[1].to_bytes(2, byteorder="little", signed=False))
            file.write(f[2].to_bytes(2, byteorder="little", signed=False))

            print(f" . Wrote {f}, 2 bytes each")

    def __save_uvs(self, file) -> None:
        print("\n4. Saving UVs...")

        for t in self.uvs:
            file.write(t[0].to_bytes(2, byteorder="little", signed=False))
            file.write(t[1].to_bytes(2, byteorder="little", signed=False))

            print(f" . Wrote {t}, 1 byte each")

    def __save_uvidxs(self, file) -> None:
        print("\n5. Saving UV Indices...")

        for t in self.uvidxs:
            file.write(t[0].to_bytes(2, byteorder="little", signed=False))
            file.write(t[1].to_bytes(2, byteorder="little", signed=False))
            file.write(t[2].to_bytes(2, byteorder="little", signed=False))

            print(f" . Wrote {t}, 2 bytes each")

    def __save_normals(self, file) -> None:
        print("\n6. Saving normals...")

        for n in self.normals:
            file.write(n[0].to_bytes(2, byteorder="little", signed=True))
            file.write(n[1].to_bytes(2, byteorder="little", signed=True))
            file.write(n[2].to_bytes(2, byteorder="little", signed=True))

            print(f" . Wrote {n}, 2 bytes each")

    def __save_nidxs(self, file) -> None:
        print("\n7. Saving normal indices...")

        for n in self.nidxs:
            file.write(n[0].to_bytes(2, byteorder="little", signed=True))
            file.write(n[1].to_bytes(2, byteorder="little", signed=True))
            file.write(n[2].to_bytes(2, byteorder="little", signed=True))

            print(f" . Wrote {n}, 2 bytes each")

    def save(self) -> None:
        print(f"Saving mesh to {self.output}:\n")

        # Refer to dev/format.txt for the format used here
        with open(self.output, "wb") as f:
            self.__save_count(f)
            self.__save_verts(f)
            self.__save_faces(f)

            if self.ext == ".mt":
                self.__save_uvs(f)
                self.__save_uvidxs(f)
                self.__save_normals(f)
                self.__save_nidxs(f)


mesh = Mesh(sys.argv)

with open(mesh.name) as f:
    for line in f.readlines():
        line = line.strip().split()

        if not line or line[0] == "#":
            continue

        if line[0] == "v":
            mesh.add_vert(line[1:])
        elif line[0] == "f":
            mesh.add_face(line[1:])
        elif line[0] == "vt":
            mesh.add_uv(line[1:])
        elif line[0] == "vn":
            mesh.add_normals(line[1:])

mesh.save()
