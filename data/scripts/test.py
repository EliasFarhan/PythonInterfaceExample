from SFGE import *
from apport import __init__


class Test(Component):
    
    def Init(self):
        print("Init Test Component from Python")
    def Update(self):
        print("Update")