from fenics import *

class Electric_field:
    def __init__(self):
        p1 = Point(-1, -1, -1)
        p2 = Point(1, 1, 1)

        mesh = BoxMesh(p1, p2, 10, 10, 10)

        p1 = Point(-1, -1, -1)
        p2 = Point(1, 1, 1)

        mesh = BoxMesh(p1, p2, 10, 10, 10)

        self.W = VectorFunctionSpace(mesh, 'CG', 1)

        self.electric_field = project(Expression(('1e4*x[0]', '1e4*x[1]', '0'), degree=2), self.W)

    def get_electric_field(self, x, y, z):
        p = Point(x, y, z)
        Ex, Ey, Ez = self.electric_field(p)
        return (Ex, Ey, Ez)
