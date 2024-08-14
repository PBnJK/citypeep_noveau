# .obj converter

import sys


class Mesh:
    def __init__(self, args: str) -> None:
        self.verts: list[list[int]] = []
        self.vcolors: list[list[int]] = []
        self.faces: list[list[int]] = []
        self.uvs: list[list[int]] = []
        self.uvidxs: list[list[int]] = []
        self.normals: list[list[int]] = []
        self.nidxs: list[list[int]] = []

        self.name: str = args[1]

        self.TYPE_MAP = {
            "f3": 0,
            "g3": 1,
            "ft3": 2,
            "gt3": 3,
        }

        self.mtype: str = args[2].lower()
        if self.mtype not in ["f3", "ft3", "g3", "gt3"]:
            print(f"whaddaheck is a '{self.mtype}' model?!")
            sys.exit(1)

        self.output: str = self.name[:-4] + ".m"

        self.h_output: str = self.name[:-4] + ".h"

        self.h_name: str = self.name[:-4].replace("/", "_").upper()
        self.h_guard: str = f"GUARD_CITYPEEP_DATA_MODEL_{self.h_name}_H_"

    def is_textured(self) -> bool:
        return self.mtype in ["ft3", "gt3"]

    def is_flat(self) -> bool:
        return self.mtype in ["f3", "ft3"]

    def is_gouraud(self) -> bool:
        return not self.is_flat()

    @staticmethod
    def __psx_vert(v: str) -> int:
        return int(float(v) * 16.0)  # TODO: make scale controllable

    @staticmethod
    def __psx_color(c: str) -> int:
        return int(float(c) * 255.0)  # 0-1 > 0-255

    def add_vert(self, v_list: list[str]) -> None:
        if self.is_flat():
            self.verts.append([self.__psx_vert(v) for v in v_list])
            return

        if len(v_list) != 6:
            print(f"Bad v_list {v_list}, has no vertex colors!")
            sys.exit(1)

        vert_list: list[str] = v_list[:3]
        self.verts.append([self.__psx_vert(v) for v in vert_list])

        color_list: list[str] = v_list[3:]
        self.vcolors.append([self.__psx_color(c) for c in color_list])

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
        if "//" in f_list[0]:
            f2_list = []
            n_list = []

            for v in f_list:
                v = v.split("//")
                f2_list.append(v[0])
                n_list.append(v[1])

            f_list = f2_list

            self.add_nidxs(n_list)

        elif "/" in f_list[0]:
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
        self.uvs.append(
            [
                self.__psx_uv(t_list[0]),
                4096 - self.__psx_uv(t_list[1]),
            ]
        )

    def __save_header(self, file) -> None:
        print("0. Saving header...")

        file.write(
            self.TYPE_MAP[self.mtype].to_bytes(2, byteorder="little", signed=False)
        )
        print(f" . Wrote type {self.mtype} ({self.TYPE_MAP[self.mtype]}), 2 bytes")

        file.write(b"\x00\x00")
        print(" . Wrote padding, 2 bytes")

    def __save_count(self, file) -> None:
        print("\n1. Saving count...")

        self.vcount = len(self.verts)
        self.fcount = len(self.faces)
        self.ncount = len(self.normals)

        if self.vcount % 2:
            self.verts.append([0, 0, 0])
            self.vcount += 1

        if self.fcount % 2:
            self.faces.append([0, 0, 0])
            self.fcount += 1

        if self.ncount % 2:
            self.normals.append([0, 0, 0])
            self.ncount += 1

        file.write(self.vcount.to_bytes(4, byteorder="little", signed=False))
        print(f" . Wrote vcount {self.vcount}, 4 bytes")

        file.write(self.fcount.to_bytes(4, byteorder="little", signed=False))
        print(f" . Wrote fcount {self.fcount}, 4 bytes")

        file.write(self.ncount.to_bytes(4, byteorder="little", signed=False))
        print(f" . Wrote ncount {self.ncount}, 4 bytes")

        if self.is_textured():
            self.tcount = len(self.uvs)

            if self.tcount % 2:
                self.tcount.append([0, 0, 0])
                self.tcount += 1

            file.write(self.tcount.to_bytes(4, byteorder="little", signed=False))
            print(f" . Wrote tcount {self.tcount}, 4 bytes")

    def __save_verts(self, file) -> None:
        print("\n2. Saving verts...")

        for count, v in enumerate(self.verts):
            file.write(v[0].to_bytes(2, byteorder="little", signed=True))
            file.write(v[1].to_bytes(2, byteorder="little", signed=True))
            file.write(v[2].to_bytes(2, byteorder="little", signed=True))

            print(f" . Wrote {v}, 2 bytes each")

            if self.is_gouraud():
                c = self.vcolors[count]
                file.write(c[0].to_bytes(1, byteorder="little", signed=False))
                file.write(c[1].to_bytes(1, byteorder="little", signed=False))
                file.write(c[2].to_bytes(1, byteorder="little", signed=False))
                file.write((0).to_bytes(1, byteorder="little", signed=False))

                print(f" . Wrote color {c} + 0, 1 byte each")

    def __save_faces(self, file) -> None:
        print("\n3. Saving faces...")

        for f in self.faces:
            file.write(f[0].to_bytes(2, byteorder="little", signed=False))
            file.write(f[1].to_bytes(2, byteorder="little", signed=False))
            file.write(f[2].to_bytes(2, byteorder="little", signed=False))

            if self.is_flat():
                file.write((80).to_bytes(1, byteorder="little", signed=False))
                file.write((80).to_bytes(1, byteorder="little", signed=False))
                file.write((80).to_bytes(1, byteorder="little", signed=False))
                file.write((0).to_bytes(1, byteorder="little", signed=False))

                print(f" . Wrote {f}, 2 bytes each")
                print(f" . Wrote {b"\x80\x80\x80\x00"}")

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
            file.write(n[0].to_bytes(2, byteorder="little", signed=False))
            file.write(n[1].to_bytes(2, byteorder="little", signed=False))
            file.write(n[2].to_bytes(2, byteorder="little", signed=False))

            print(f" . Wrote {n}, 2 bytes each")

    def save(self) -> None:
        print(f"Saving mesh to {self.output}:\n")

        # Refer to dev/format.txt for the format used here
        with open(self.output, "wb") as f:
            self.__save_header(f)
            self.__save_count(f)
            self.__save_verts(f)
            self.__save_faces(f)
            self.__save_normals(f)
            self.__save_nidxs(f)

            if self.is_textured():
                self.__save_uvs(f)
                self.__save_uvidxs(f)

    def __hsave_header(self, file) -> None:
        print("0. Saving header...")

        file.write("\t.rot={0, 0, 0},\n")
        file.write("\t.trans={0, 0, 0},\n")
        file.write("\t.scale={4096, 4096, 4096},\n\n")

        file.write(f"\t.type={self.TYPE_MAP[self.mtype]},\n")
        file.write("\t.flags={.visible=1},\n\n")

    def __hsave_verts(self, file) -> None:
        print("\n1. Saving verts...")

        file.write(f"\t.vcount={self.vcount},\n")
        file.write("\t.verts={\n")

        for v in self.verts:
            file.write(f"\t\t{{ {v[0]}, {v[1]}, {v[2]}, 0 }},\n")

        file.write("\t},\n\n")

    def __hsave_colors(self, file) -> None:
        print("\n2. Saving colors...")

        file.write(f"\t.ccount={self.ccount},\n")
        file.write("\t.colors={\n")

        for c in self.verts:
            file.write(f"\t\t{{ {c[0]}, {c[1]}, {c[2]}, 0 }},\n")

        file.write("\t},\n\n")

    def __hsave_faces(self, file) -> None:
        print("\n3. Saving faces...")

        file.write(f"\t.fcount={self.fcount},\n")
        file.write("\t.faces={\n")

        for f in self.faces:
            file.write(f"\t\t{{ {f[0]}, {f[1]}, {f[2]}, 0 }},\n")

        file.write("\t},\n\n")

    def __hsave_uvs(self, file) -> None:
        print("\n4. Saving UVs...")

        file.write(f"\t.tcount={self.tcount},\n")
        file.write("\t.uvs={\n")

        for t in self.uvs:
            file.write(f"\t\t{{ {t[0]}, {t[1]}, 0 }},\n")

        file.write("\t},\n\n")

    def __hsave_uvidxs(self, file) -> None:
        print("\n5. Saving UV Indices...")

        file.write("\t.uvidxs={\n")

        for t in self.uvidxs:
            file.write(f"\t\t{{ {t[0]}, {t[1]}, {t[2]}, 0 }},\n")

        file.write("\t},\n\n")

    def __hsave_normals(self, file) -> None:
        print("\n6. Saving normals...")

        file.write(f"\t.ncount={self.ncount},\n")
        file.write("\t.normals={\n")

        for n in self.normals:
            file.write(f"\t\t{{ {n[0]}, {n[1]}, {n[2]}, 0 }},\n")

        file.write("\t},\n\n")

    def __hsave_nidxs(self, file) -> None:
        print("\n7. Saving normal indices...")

        file.write("\t.nidxs={\n")

        for n in self.nidxs:
            file.write(f"\t\t{{ {n[0]}, {n[1]}, {n[2]}, 0 }},\n")

        file.write("\t},\n\n")

    def save_to_header(self) -> None:
        print(f"Saving mesh to {self.h_output}:\n")

        with open(self.h_output, "w") as f:
            f.write(f"#ifndef {self.h_guard}\n")
            f.write(f"#define {self.h_guard}\n\n")

            f.write('#include "gfx.h"\n\n')

            f.write(f"static const CP_Mesh {self.h_name} = {{\n")

            self.vcount = len(self.verts)
            self.ccount = len(self.vcolors)
            self.fcount = len(self.faces)
            self.ncount = len(self.normals)

            if self.is_textured():
                self.tcount = len(self.uvs)

            self.__hsave_header(f)
            self.__hsave_verts(f)
            self.__hsave_colors(f)
            self.__hsave_faces(f)
            self.__hsave_normals(f)
            self.__hsave_nidxs(f)

            # if self.is_textured():
            #    self.__hsave_uvs(f)
            #    self.__hsave_uvidxs(f)

            f.write("};\n\n")

            f.write(f"#endif // !{self.h_guard}\n")


if len(sys.argv) != 3:
    print("usage: python image_conv.py [obj] [type]")
    print("- obj: The model to convert, in .OBJ file format")
    print("- type: One of F3, FT3, G3 or GT3")

    sys.exit(1)

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
        elif line[0] == "vn":
            mesh.add_normals(line[1:])

        if mesh.is_textured():
            if line[0] == "vt":
                mesh.add_uv(line[1:])

mesh.save()
mesh.save_to_header()
