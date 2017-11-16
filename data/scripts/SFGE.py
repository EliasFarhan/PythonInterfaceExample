'''
Created on Nov 15, 2017

@author: efarhan
'''

print("PYTHON SFGE FILE")

class Component:
    def Init(self):
        pass
    def Update(self):
        pass


class Vector():
    def __init__(self,x,y):
        self.x = x
        self.y = y
    def __init__(self):
        self.x = 0
        self.y = 0
    def length(self):
        pass

class Transform(Component):
    def Init(self):
        self.pos = Vector()


class Camera(Component):
    def Init(self):
        self.transform = Transform()
        self.transform.p