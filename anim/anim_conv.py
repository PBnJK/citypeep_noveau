# Converts animation XML into .ANI files

import sys


class Anim:
    def __init__(self, args: list[str]) -> None:
        self.name: str = args[1]
        self.output: str = self.name[:-4] + ".ani"


anim: Anim = Anim(sys.argv)
anim.save()
