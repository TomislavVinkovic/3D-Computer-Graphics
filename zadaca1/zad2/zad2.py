"""
KAKO SAM DOSAO DO RJESENJA:
generirao sam tocke i lica pomocu skripte, i spremio ih u cilindar.obj. Zatim sam generirani obj ubacio u blender,
koji je za mene izracunao normale. Onda sam samo ponovno exportao obj datoteku, te maknuo materijal
"""

import math

class Vector:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
    def __str__(self):
        return str((self.x,self.y,self.z))

class Circle:
    def __init__(self, dots = 20, z=0):
        self.dots = dots
        self.res = 360/self.dots
        self.vertices = []
        self.normals = []
        self.faces = []
        self.radius = 1 #jedinicna kruznica
        self.generateCircle(0, z)
        #self.generateNormals(0)
        self.generateFaces()

    def generateCircle(self, angle, z):
        if angle >= 360:
            return
        x = math.cos((angle / 360)*2*self.radius*math.pi)
        y = math.sin((angle / 360)*2*self.radius*math.pi)
        self.vertices.append(Vector(x, z, y))
        self.generateCircle((angle + self.res), z)
    
    def generateNormals(self, angle, defaultVectorGenerated=False):
        if angle >= 360:
            return
        #hardkodirane normale, koje nisu radile
        x = math.cos((angle / 360)*2*self.radius*math.pi)
        y = math.sin((angle / 360)*2*self.radius*math.pi)
        self.normals.append(Vector(y,0,x))
        #self.normals.append(Vector(0,0,1))
        self.generateNormals(angle + self.res, True)

    def generateFaces(self):
        n = len(self.vertices)
        for i in range(1, n-1):
            #i am increasing the indices by one because indices 		in obj start from 1, not 0
            self.faces.append((1, i+1, i+2))

    def writeOut(self, fileName):
        with open(fileName, "w") as f:
            for v in self.vertices:
                f.write(f"v {v.x} {v.y} {v.z}\n")
            
            for n in self.normals:
                f.write(f"vn {n.x} {n.y} {n.z}\n")
            
            i = 1
            for face in self.faces:
                f.write(f"f {face[0]} {face[1]} {face[2]}\n")
                i+=1

    def __str__(self):
        return str(self.vertices)


class Cylinder:
    def __init__(self, upperBase: Circle, lowerBase: Circle):
        self.upperBase = upperBase
        self.lowerBase = lowerBase
        self.vertices = upperBase.vertices + lowerBase.vertices #concatenate their vertices 
        #self.normals = upperBase.normals + lowerBase.normals #same for normals
        #self.normals.insert(0, Vector(1, 0, 0))
        #self.faces = upperBase.faces + lowerBase.faces #and faces...
        faces_length = len(self.lowerBase.faces) + len(self.upperBase.faces)
        self.faces = [] #side faces
        lowerBaseMainVertexIndex = faces_length // 2
        #print(lowerBaseMainVertexIndex)
        for i in range(0, len(self.lowerBase.faces)):
            self.lowerBase.faces[i] = (lowerBaseMainVertexIndex+3, self.lowerBase.faces[i][1] + lowerBaseMainVertexIndex+2, self.lowerBase.faces[i][2] + lowerBaseMainVertexIndex+2)
        self.generateFaces()
    def generateFaces(self):
        lowerBaseVerticesN = len(self.lowerBase.vertices)
        for i, v in enumerate(self.upperBase.vertices):
            #prokuzeno eksperimentiranjem
            self.faces.append((i+1, lowerBaseVerticesN+i, lowerBaseVerticesN+i+1))
            self.faces.append((i+1, lowerBaseVerticesN+i+1, i+2))
    def writeOut(self, fileName):
        base_faces = self.upperBase.faces + self.lowerBase.faces
        with open(fileName, "w") as f:
            for v in self.vertices:
                f.write(f"v {v.x} {v.y} {v.z}\n")
            #normals are calculated in blender instead
            """
            for n in self.normals:
                f.write(f"vn {n.x} {n.y} {n.z}\n")
            """
            for face in base_faces:
                f.write(f"f {face[0]} {face[1]} {face[2]}\n")
            i=2
            j=1
            for face in self.faces:
                f.write(f"f {face[0]} {face[1]} {face[2]}\n")
                if j==2:
                    i+=1
                    j=1
                else:
                    j+=1
    #def writeOut(self, filename:str):


if __name__ == "__main__":
    upperBase = Circle(dots = 60)
    lowerBase = Circle(dots = 60, z=2)
    c = Cylinder(upperBase=upperBase, lowerBase=lowerBase)
    #print(c.faces)
    #print(upperBase.faces)
    c.writeOut("cilindar.obj")
    #c.writeOut("circle.obj")
    
